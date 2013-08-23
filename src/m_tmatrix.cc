/* Copyright (C) 2013 Oliver Lemke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*!
  \file   m_tmatrix.cc
  \author Oliver Lemke
  \date   2013-06-25

  \brief  T-Matrix related workspace methods.
*/

#include <stdexcept>
#include <cmath>
#include "messages.h"
#include "tmatrix.h"
#include "check_input.h"
#include "make_array.h"
#include "refraction.h"
#include "special_interp.h"



void TMatrixTest(const Verbosity& verbosity)
{
    tmatrix_tmd_test(verbosity);
    tmatrix_ampld_test(verbosity);
    calc_ssp_random_test(verbosity);
    calc_ssp_fixed_test(verbosity);
}
//-----------------------------
void complex_refr_indexRegrid(//WS Output:
                 GriddedField3& complex_refr_index,
                 //WS Generic input
                 const Vector& scat_f_grid,
                 const Vector& scat_T_grid,
                 const Verbosity&)
{
  GriddedField3 complex_refr_index_temp = complex_refr_index;
    
  const Index nf = scat_f_grid.nelem();
  const Index nt = scat_T_grid.nelem();
    
  complex_refr_index.resize( nf, nt, 2 );
  complex_refr_index.set_grid_name( 0, "Frequency" );
  complex_refr_index.set_grid( 0, scat_f_grid );
  complex_refr_index.set_grid_name( 1, "Temperature" );
  complex_refr_index.set_grid( 1, scat_T_grid );
  complex_refr_index.set_grid_name( 2, "Complex" );
  complex_refr_index.set_grid( 2, MakeArray<String>("real", "imaginary") );
  
  complex_n_interp(complex_refr_index.data(joker, joker,0), complex_refr_index.data(joker, joker,1),
                   complex_refr_index_temp, "complex_refr_index", scat_f_grid, scat_T_grid);
}

//-----------------------------------


void scat_meta_arrayInit(// WS Output:
                 ArrayOfScatteringMetaData& scat_meta_array,
                 //WS Input
                 const Verbosity&)
{
    scat_meta_array.resize(0);
}

                
                 
void scat_meta_arrayAddTmatrix(// WS Output:
                             ArrayOfScatteringMetaData& scat_meta_array,
                             // SW Input:
                             const GriddedField3& complex_refr_index,
                             // WS Generic input:
                             const String& description,
                             const String& material,
                             const String& shape,
                             const String& particle_type,
                             const Numeric& density,
                             const Numeric& aspect_ratio,
                             const Vector& diameter_grid,
                             const Vector& scat_f_grid,
                             const Vector& scat_T_grid,
                             const Verbosity&) 

{
  chk_if_equal("scat_f_grid", "data_f_grid from complex_refr_index", scat_f_grid, complex_refr_index.get_numeric_grid(0));
  chk_if_equal("scat_T_grid", "data_T_grid from complex_refr_index", scat_T_grid, complex_refr_index.get_numeric_grid(1));

  for(Index k=0; k<diameter_grid.nelem(); k++)
    {
      extern const Numeric PI;

      Numeric diameter_max;

      if (shape == "spheroidal")

        if (aspect_ratio<1)
          diameter_max=diameter_grid[k]*pow(aspect_ratio, -2./3.);

        else if (aspect_ratio>1)
          diameter_max=diameter_grid[k]*pow(aspect_ratio, 1./3.);

        else
          {
            ostringstream os;
            os << "Incorrect aspect ratio: " << aspect_ratio << "\n"
              << "Can not be equal to one";
            throw std::runtime_error(os.str());
          }

      else if (shape == "cylindrical")

          diameter_max=pow(pow(16./3., 2./3.)*pow(diameter_grid[k]/.2, 2.)*
                    (pow(aspect_ratio, -4./3.)+pow(aspect_ratio, 2./3.)), 1./2.);
 

      else
        {
          ostringstream os;
          os << "Unknown particle shape: " << shape << "\n"
            << "Must be spheroidal or cylindrical";
          throw std::runtime_error(os.str());
        }

      ScatteringMetaData smd;
      if (description=="")
        {   
          ostringstream os;
          os << shape<< " "<< material << " particle of type " << particle_type<<
            ", with volume equivalent diameter "
            <<diameter_grid[k]<<" meters.";
          smd.description=os.str();
        }
      else 
        smd.description = description;
 
      smd.material = material;
      smd.shape = shape;
      smd.particle_type = ParticleTypeFromString(particle_type);
      smd.ssd_method = PARTICLE_SSDMETHOD_TMATRIX;
      smd.density = density;
      smd.diameter_max =diameter_max;
      smd.volume = 4./3.*PI*pow(diameter_grid[k]/2., 3);
      smd.area_projected = 0;
      smd.aspect_ratio = aspect_ratio;
      smd.scat_f_grid = scat_f_grid;
      smd.scat_T_grid = scat_T_grid;
      smd.complex_refr_index = complex_refr_index.data;

      scat_meta_array.push_back(smd);
    }
}

//-----------------------------------


void scat_data_arrayFromMeta(// WS Output:
                              ArrayOfSingleScatteringData& scat_data_array,
                              //WS Input
                              const ArrayOfScatteringMetaData& scat_meta_array,
                              // WS Generic input:
                              const Vector& za_grid,
                              const Vector& aa_grid,
                              const Numeric& precision,
                              const Verbosity&)
{
  for(Index ii=0;ii<scat_meta_array.nelem();ii++)
    {

      extern const Numeric PI;  
      Index  np;

      SingleScatteringData sdd;
      sdd.f_grid = scat_meta_array[ii].scat_f_grid;
      sdd.T_grid = scat_meta_array[ii].scat_T_grid;
      sdd.za_grid = za_grid;
      sdd.aa_grid = aa_grid;
      sdd.particle_type = scat_meta_array[ii].particle_type;

      if (scat_meta_array[ii].shape == "spheroidal" )
        np=-1;

      else if (scat_meta_array[ii].shape == "cylindrical")
        np=-2;
      else
        {
          ostringstream os;
          os << "Unknown particle shape: " << scat_meta_array[ii].shape << "\n"
            << "Must be spheroidal or cylindrical";
          throw std::runtime_error(os.str());
        }

      calcSingleScatteringDataProperties(sdd,
                                         scat_meta_array[ii].complex_refr_index(joker,joker,0),
                                         scat_meta_array[ii].complex_refr_index(joker,joker,1),
                                         pow(scat_meta_array[ii].volume*1e18*3./(4.*PI),1./3.),
                                         np,
                                         scat_meta_array[ii].aspect_ratio,
                                         precision);

      scat_data_array.push_back(sdd);
    }
}

