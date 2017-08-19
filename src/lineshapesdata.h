/* Copyright (C) 2017
 * Richard Larsson <ric.larsson@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA. */

/*!
 * \file   lineshapesdata.h
 * \brief  Stuff related to lineshape functions.
 * 
 * This file should contain complete handling of individual lines.
 * The reason is that the old methods are cumbersome to adapt and need redesigning
 * 
 * \author Richard Larsson
 * \date   2017-05-16
 */

#ifndef linefunctions_h
#define linefunctions_h

#include "complex.h"
#include "partial_derivatives.h"


/*
 * Class to solve the problem
 * 
 *      cross-section of a line equals line strength times line shape
 *      
 *      or
 * 
 *      sigma = S x F,
 * 
 * which means
 *      
 *      dsigma = dS x F + S x dF
 * 
 * TODO: Add QuantumIdentifier input to test for line catalog-parameters derivatives in relevant places
 * 
 * TODO: Add NLTE line strength calculator
 * 
 * TODO: Better combination with Zeeman calculations
 * 
 * TODO: Find work-around for incomplete line-shapes like "Voigt Kuntz"
 */


namespace Linefunctions
{

  /*!
   * 
   * 
   * 
   */
  
  enum class Lineshape
  {
    None,                       // Holder that should always fail if evoked  X
    O2_resonant,                // Debye lineshape  X
    Doppler,                    // Doppler lineshape  X
    Lorentz,                    // Lorentz lineshape  X
    Mirrored_lorentz,           // Mirrored Lorentz lineshape  X
    Faddeeva_algorithm916,      // Faddeeva lineshape  X
    Faddeeva_Hui_etal_1979,     // Faddeeva lineshape  X
    Hartmann_and_Tran_profile   // Hartmann-Tran lineshape  X
  }; 
  
  enum class Linenormalization
  {
    None,                   // No normalization  X
    Rosenkranz_quadratic,   // Quadratic normalization (f/f0)^2*h*f0/(2*k*T)/sinh(h*f0/(2*k*T))  X
    Van_Vleck_and_Weiskopf, // Van Vleck Weiskopf normalization (f*f) / (f0*f0)  X
    Van_Vleck_and_Huber     // Van Vleck Huber normalization f*tanh(h*f/(2*k*T))) / (f0*tanh(h*f0/(2*k*T)))  X
  };
  
  void set_lorentz(ComplexVector& F, // Sets the full complex line shape without line mixing
                   ArrayOfComplexVector& dF,
                   const Vector& f_grid,
                   const Numeric& zeeman_df,
                   const Numeric& magnetic_magnitude,
                   const Numeric& F0_noshift,
                   const Numeric& G0,
                   const Numeric& L0,
                   const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                   const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                   const Numeric& dG0_dT=0.0,
                   const Numeric& dL0_dT=0.0);
  
  void set_mirrored_lorentz(ComplexVector& F, // Sets the full complex line shape without line mixing
                            ArrayOfComplexVector& dF, 
                            const Vector& f_grid,
                            const Numeric& zeeman_df,
                            const Numeric& magnetic_magnitude,
                            const Numeric& F0_noshift,
                            const Numeric& G0,
                            const Numeric& L0,
                            const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                            const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                            const Numeric& dG0_dT=0.0,
                            const Numeric& dL0_dT=0.0);
  
  void set_htp(ComplexVector& F, // Sets the full complex line shape without line mixing
               ArrayOfComplexVector& dF,
               const Vector& f_grid,
               const Numeric& zeeman_df,
               const Numeric& magnetic_magnitude,
               const Numeric& F0_noshift,
               const Numeric& GD_div_F0,
               const Numeric& G0,
               const Numeric& L0,
               const Numeric& L2,
               const Numeric& G2,
               const Numeric& eta,
               const Numeric& FVC,
               const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
               const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
               const Numeric& dGD_div_F0_dT=0.0,
               const Numeric& dG0_dT=0.0,
               const Numeric& dL0_dT=0.0,
               const Numeric& dG2_dT=0.0,
               const Numeric& dL2_dT=0.0,
               const Numeric& deta_dT=0.0,
               const Numeric& dFVC_dT=0.0);
  
  void set_faddeeva_algorithm916(ComplexVector& F, // Sets the full complex line shape without line mixing
                                 ArrayOfComplexVector& dF,
                                 const Vector& f_grid,
                                 const Numeric& zeeman_df,
                                 const Numeric& magnetic_magnitude,
                                 const Numeric& F0_noshift,
                                 const Numeric& GD_div_F0,
                                 const Numeric& G0,
                                 const Numeric& L0,
                                 const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                                 const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                                 const Numeric& dGD_div_F0_dT=0.0,
                                 const Numeric& dG0_dT=0.0,
                                 const Numeric& dL0_dT=0.0);
  
  void set_doppler(ComplexVector& F, // Sets the full complex line shape without line mixing
                   ArrayOfComplexVector& dF,
                   const Vector& f_grid,
                   const Numeric& zeeman_df,
                   const Numeric& magnetic_magnitude,
                   const Numeric& F0_noshift,
                   const Numeric& GD_div_F0,
                   const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                   const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                   const Numeric& dGD_div_F0_dT=0.0);
  
  void set_faddeeva_from_full_linemixing(ComplexVector& F, // Sets the full complex line shape without line mixing
                                         ArrayOfComplexVector& dF,
                                         const Vector& f_grid,
                                         const Complex& eigenvalue_no_shift,
                                         const Numeric& GD_div_F0,
                                         const Numeric& L0,
                                         const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                                         const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                                         const Numeric& dGD_div_F0_dT=0.0,
                                         const Complex& deigenvalue_dT=0.0,
                                         const Numeric& dL0_dT=0.0);
  
  
  void set_hui_etal_1978(ComplexVector& F, // Sets the full complex line shape without line mixing
                         ArrayOfComplexVector& dF,
                         const Vector& f_grid,
                         const Numeric& zeeman_df,
                         const Numeric& magnetic_magnitude,
                         const Numeric& F0_noshift,
                         const Numeric& GD_div_F0,
                         const Numeric& G0,
                         const Numeric& L0,
                         const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                         const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                         const Numeric& dGD_div_F0_dT=0.0,
                         const Numeric& dG0_dT=0.0,
                         const Numeric& dL0_dT=0.0);
  
  void set_o2_non_resonant(ComplexVector& F, // Sets the full complex line shape without line mixing
                           ArrayOfComplexVector& dF,
                           const Vector& f_grid,
                           const Numeric& F0,
                           const Numeric& G0,
                           const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                           const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                           const Numeric& dG0_dT=0.0);
  
  void apply_linemixing(ComplexVector& F, // Returns the full complex or normalized line shape with line mixing
                        ArrayOfComplexVector& dF,
                        const Numeric& Y,
                        const Numeric& G,
                        const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                        const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                        const Numeric& dY_dT=0.0,
                        const Numeric& dG_dT=0.0);
  
  void apply_rosenkranz_quadratic(ComplexVector& F, // Returns as normalized complex line shape with or without line mixing
                                  ArrayOfComplexVector& dF,
                                  const Vector& f_grid,
                                  const Numeric& F0, // Only line center without any shifts
                                  const Numeric& T,
                                  const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                                  const QuantumIdentifier& quantum_identity=QuantumIdentifier());
  
  void apply_VVH(ComplexVector& F, // Returns as normalized complex line shape with or without line mixing
                 ArrayOfComplexVector& dF,
                 const Vector& f_grid,
                 const Numeric& F0, // Only line center without any shifts
                 const Numeric& T,
                 const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                 const QuantumIdentifier& quantum_identity=QuantumIdentifier());
  
  void apply_VVW(ComplexVector& F, // Returns as normalized line shape with or without line mixing
                 ArrayOfComplexVector& dF,
                 const Vector& f_grid,
                 const Numeric& F0, // Only line center without any shifts
                 const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                 const QuantumIdentifier& quantum_identity=QuantumIdentifier());
  
  void apply_linestrength(ComplexVector& F, // Returns the full complex line shape with or without line mixing
                          ArrayOfComplexVector& dF,
                          const Numeric& S0,
                          const Numeric& isotopic_ratio,
                          const Numeric& QT,
                          const Numeric& QT0,
                          const Numeric& K1,
                          const Numeric& K2,
                          //const Numeric& K3,  Add again when NLTE is considered
                          //const Numeric& K4,  Add again when NLTE is considered
                          const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                          const QuantumIdentifier& quantum_identity=QuantumIdentifier(),
                          const Numeric& dQT_dT=0.0,
                          const Numeric& dK1_dT=0.0,
                          const Numeric& dK2_dT=0.0,
                          const Numeric& dK2_dF0=0.0); // Add all other derivatives related to NLTE
  
  void apply_linestrength_from_full_linemixing(ComplexVector& F, // Returns the full complex line shape with line mixing
                                               ArrayOfComplexVector& dF,
                                               const Numeric& F0,
                                               const Numeric& T,
                                               const Complex& S_LM,
                                               const Numeric& isotopic_ratio,
                                               const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                                               const Complex& dS_LM_dT=0.0);
  
  void apply_dipole(ComplexVector& F, // Returns the full complex line shape without line mixing
                    ArrayOfComplexVector& dF, // Returns the derivatives of the full line shape for list_of_derivatives
                    const Numeric& F0,
                    const Numeric& T,
                    const Numeric& d0,
                    const Numeric& rho,
                    const Numeric& isotopic_ratio,
                    const PropmatPartialsData& derivatives_data=PropmatPartialsData(),
                    const Numeric& drho_dT=0.0);
  
  void apply_pressurebroadening_jacobian(ArrayOfComplexVector& dF,
                                         const PropmatPartialsData& derivatives_data,
                                         const QuantumIdentifier& quantum_identity,
                                         const Vector& dgamma);
  
  Numeric DopplerConstant(const Numeric T, const Numeric mass);
  
  Numeric dDopplerConstant_dT(const Numeric T, const Numeric mass);
};

#endif //lineshapedata_h