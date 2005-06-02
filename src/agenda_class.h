/* Copyright (C) 2000, 2001 Stefan Buehler <sbuehler@uni-bremen.de>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

/*!
  \file   agenda_class.h
  \author Stefan Buehler <sbuehler@uni-bremen.de>
  \date   Thu Mar 14 08:49:33 2002
  
  \brief  Declarations for agendas.
*/

#ifndef agenda_class_h
#define agenda_class_h

#include "token.h"
#include "set"

// ... and MRecord
class MRecord;

//! The Agenda class.
/*! An agenda is a list of workspace methods (including keyword data)
  to be executed. There are workspace variables of class agenda that
  can contain a list of methods to execute for a particular purpose,
  for example to compute the lineshape in an absorption
  calculation. 
*/
class Agenda {
public:
  void push_back(MRecord n);
  void execute(bool silent=false) const;
  void resize(Index n);
  Index nelem() const;
  Agenda& operator=(const Agenda& x);
  void get_outputs_to_push_and_dup (set<Index> &outputs_to_push,
                                    set<Index> &outputs_to_dup) const;
  bool is_input(Index var) const;
  bool is_output(Index var) const;
  void set_name(const String& nname);
  String name() const;
  void print( ostream& os,
              const String& indent ) const;
private:
  String         mname; /*!< Agenda name. */
  Array<MRecord> mml;   /*!< The actual list of methods to execute. */
};

// Documentation with implementation.
ostream& operator<<(ostream& os, const Agenda& a);


/** Method runtime data. In contrast to MdRecord, an object of this
    class contains the runtime information for one method: The method
    id and the keyword parameter values. This is all that the engine
    needs to execute the stack of methods.

    An MRecord includes a member magenda, which can contain an entire
    agenda, i.e., a list of other MRecords. 

    @author Stefan Buehler */
class MRecord {
public:
  MRecord(){ /* Nothing to do here. */ }
  MRecord(const Index id,
          const Array<TokVal>& values,
          const ArrayOfIndex& output,
          const ArrayOfIndex& input,
          const Agenda&       tasks)
    : mid(id),
      mvalues( values ),
      moutput( output ),
      minput(  input  ),
      mtasks( tasks )
  { 
    // Initialization of arrays from other array now works correctly.
  }
  Index                Id()       const { return mid;     }
  const Array<TokVal>& Values()   const { return mvalues; }
  const ArrayOfIndex&  Output()   const { return moutput; }
  const ArrayOfIndex&  Input()    const { return minput;  }
  const Agenda&        Tasks()    const { return mtasks;  }

  //! Assignment operator for MRecord.
  /*! 
    This is necessary, because it is used implicitly if agendas (which
    contain an array of MRecord) are copied. The default assignment
    operator generated by the compiler does not do the right thing!

    This became clear due to a bug when agendas were re-defined in the
    controlfile, which was discoverd by Patrick.

    The problem is that MRecord contains some arrays. The copy semantics
    for Array require the target Array to have the right size. But if we
    overwrite an old MRecord with a new one, we want all arrays to be
    overwritten. We don't care about their old size.

    \param x The other MRecord to assign.

    \return The freshly assigned MRecord.

    \author Stefan Buehler
    \date   2002-12-02
    */
  MRecord& operator=(const MRecord& x)
    {
      mid = x.mid;

      mvalues.resize(x.mvalues.nelem());
      mvalues = x.mvalues;

      moutput.resize(x.moutput.nelem());
      moutput = x.moutput;

      minput.resize(x.minput.nelem());
      minput = x.minput;

      mtasks.resize(x.mtasks.nelem());
      mtasks = x.mtasks;

      return *this;
    }

  // Output operator:
  void                 print( ostream& os,
                              const String& indent ) const;

private:
  /** Method id. */
  Index mid;
  /** List of parameter values (see methods.h for definition of
      TokVal). */
  Array<TokVal> mvalues;
  /** Output workspace variables (for generic methods). */
  ArrayOfIndex moutput;
  /** Input workspace variables (for generic methods). */
  ArrayOfIndex minput;
  /** An agenda, which can be given in the controlfile instead of
      keywords. */
  Agenda mtasks;
};

//! Set size to n.
inline void Agenda::resize(Index n)
{
  mml.resize(n);
}

//! Return the number of agenda elements.
/*!  
  This is needed, so that we can find out the correct size for
  resize, befor we do a copy.

  \return Number of agenda elements.
*/
inline Index Agenda::nelem() const
{
  return mml.nelem();
}

//! Assignment operator.
/*! 
  Size of target must have been adjusted before, otherwise an
  assertion fails. 
*/
inline Agenda& Agenda::operator=(const Agenda& x)
{
  assert( mml.nelem() == x.mml.nelem() );
  mml = x.mml;
  return *this;
}

// Documentation is with implementation.
ostream& operator<<(ostream& os, const MRecord& a);

#endif

