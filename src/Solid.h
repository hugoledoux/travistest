/*
 val3dity - Copyright (c) 2011-2016, Hugo Ledoux.  All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the authors nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL HUGO LEDOUX BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*/

#ifndef __val3dity__Solid__
#define __val3dity__Solid__

#include "Primitive.h"
#include "definitions.h"
#include "Surface.h"

#include <vector>
#include <string>

namespace val3dity
{

class Solid : public Primitive
{
public:
  Solid(std::string id = "");
  ~Solid();
  
  Surface*        get_oshell();
  void            set_oshell(Surface* sh);
  Surface*        get_ishell(int i);
  void            add_ishell(Surface* sh);

  int             num_ishells();
  int             num_faces();
  int             num_vertices();
 
  bool            validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap = -1);
  Nef_polyhedron* get_nef_polyhedron();
  void            get_min_bbox(double& x, double& y);
  void            translate_vertices(double minx, double miny);
  std::string     get_report_xml();
  std::string     get_poly_representation();
  int             is_valid();
  bool            is_empty();
  Primitive3D     get_type();
  std::set<int>   get_unique_error_codes();
  
  const std::vector<Surface*>&  get_shells();


protected:
  std::vector<Surface*>  _shells;
  Nef_polyhedron*        _nef;

  bool validate_solid_with_nef();
};

} // namespace val3dity

#endif /* defined(__val3dity__Solid__) */
