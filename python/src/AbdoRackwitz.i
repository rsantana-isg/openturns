// SWIG file AbdoRackwitz.i

%{
#include "openturns/AbdoRackwitz.hxx"
%}

%include AbdoRackwitz_doc.i

%include openturns/AbdoRackwitz.hxx

namespace OT{ %extend AbdoRackwitz { AbdoRackwitz(const AbdoRackwitz & other) { return new OT::AbdoRackwitz(other); } } }
