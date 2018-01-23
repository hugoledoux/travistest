/*
  val3dity 

  Copyright (c) 2011-2017, 3D geoinformation research group, TU Delft  

  This file is part of val3dity.

  val3dity is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  val3dity is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with val3dity.  If not, see <http://www.gnu.org/licenses/>.

  For any information or further details about the use of val3dity, contact
  Hugo Ledoux
  <h.ledoux@tudelft.nl>
  Faculty of Architecture & the Built Environment
  Delft University of Technology
  Julianalaan 134, Delft 2628BL, the Netherlands
*/

#ifndef CompositeSurface_h
#define CompositeSurface_h

#include "Primitive.h"
#include "Surface.h"

#include <string>

namespace val3dity
{

class CompositeSurface : public Primitive 
{
public:
              CompositeSurface(std::string id = ""); 
              ~CompositeSurface(); 

  bool          validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap = -1);
  int           is_valid();
  bool          is_empty();
  json          get_report_json();
  Primitive3D   get_type();

  void          get_min_bbox(double& x, double& y);
  void          translate_vertices();

  std::string   get_off_representation();

  int           num_faces();
  int           num_vertices();
  bool          set_surface(Surface* s);
  Surface*      get_surface();
  std::set<int> get_unique_error_codes();

protected:
  Surface* _surface;
};

} // namespace val3dity

#endif /* CompositeSurface_h */
