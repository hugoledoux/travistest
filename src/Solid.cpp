
#include "Solid.h"
#include "definitions.h"

#include "input.h"
#include "validate_shell.h"

namespace val3dity
{

Solid::Solid(std::string id)
{
  _id = id;
  _is_valid = -1;
  _nef = NULL;
}


Solid::~Solid()
{}

Surface* Solid::get_oshell()
{
  return _shells[0];
}

Primitive3D Solid::get_type() 
{
  return SOLID;
}

void Solid::set_oshell(Surface* sh)
{
  if (_shells.empty())
    _shells.push_back(sh);
  else
    _shells[0] = sh;
}


const std::vector<Surface*>& Solid::get_shells()
{
  return _shells;
}


void Solid::add_ishell(Surface* sh)
{
  _shells.push_back(sh);
}


int Solid::is_valid()
{
  for (auto& sh : _shells)
  {
    if (sh->has_errors() == true)
    {
      _is_valid = 0;
      return 0;
    }
  }
  if ( (_is_valid == 1) && (this->is_empty() == false) )
    return 1;
  else
    return _is_valid;
}


bool Solid::is_empty()
{
  for (auto& sh : _shells)
  {
    if (sh->is_empty() == true)
      return true;
  }
  return false;
}


void Solid::get_min_bbox(double& x, double& y)
{
  double tmpx, tmpy;
  double minx = 9e10;
  double miny = 9e10;
  for (auto& sh : _shells)
  {
    sh->get_min_bbox(tmpx, tmpy);
    if (tmpx < minx)
      minx = tmpx;
    if (tmpy < miny)
      miny = tmpy;
  }
  x = minx;
  y = miny;
}


void Solid::translate_vertices(double minx, double miny)
{
  for (auto& sh : _shells)
    sh->translate_vertices(minx, miny);
}


bool Solid::validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap)
{
  std::clog << "- Solid validation (#" << _id << ") -" << std::endl;
  if (this->is_valid() == 0)
  {
    return false;
  }
  bool isValid = true;
  if (this->is_empty() == true)
  {
    this->add_error(902, "", "probably error while parsing the input");
    return false;
  }
  for (auto& sh : _shells)
  {
    if (sh->validate_as_shell(tol_planarity_d2p, tol_planarity_normals) == false) 
      isValid = false;
  }
  if (isValid == true) 
  {
    if (validate_solid_with_nef() == false)
      isValid = false;
  }
  _is_valid = isValid;
  return isValid;
}


std::string Solid::get_poly_representation()
{
  std::ostringstream s;
  for (auto& sh : _shells)
  {
    s << sh->get_poly_representation() << std::endl;
  }
  return s.str();
}

std::string Solid::get_report_xml()
{
  std::stringstream ss;
  ss << "\t<Solid>" << std::endl;
  if (this->get_id() != "")
    ss << "\t\t<id>" << this->_id << "</id>" << std::endl;
  else
    ss << "\t\t<id>none</id>" << std::endl;
  ss << "\t\t<numbershells>" << (this->num_ishells() + 1) << "</numbershells>" << std::endl;
  ss << "\t\t<numberfaces>" << this->num_faces() << "</numberfaces>" << std::endl;
  ss << "\t\t<numbervertices>" << this->num_vertices() << "</numbervertices>" << std::endl;
  // if (this->_inputtype == OBJ)
  // {
  //   Surface* sh = this->get_oshell();
  //   if (sh->were_vertices_merged_during_parsing() == true)
  //     ss << "\t\t<numberverticesmerged>" << (sh->get_number_parsed_vertices() - sh->number_vertices()) << "</numberverticesmerged>" << std::endl;
  // }
  // if (_id_building.empty() == false)
  //   ss << "\t\t<Building>" << this->get_id_building() << "</Building>" << std::endl;
  // if (_id_buildingpart.empty() == false)
  //   ss << "\t\t<BuildingPart>" << this->get_id_buildingpart() << "</BuildingPart>" << std::endl;
  for (auto& err : _errors)
  {
    for (auto& e : _errors[std::get<0>(err)])
    {
      ss << "\t\t<Error>" << std::endl;
      ss << "\t\t\t<code>" << std::get<0>(err) << "</code>" << std::endl;
      ss << "\t\t\t<type>" << errorcode2description(std::get<0>(err)) << "</type>" << std::endl;
      ss << "\t\t\t<shell>" << std::get<0>(e) << "</shell>" << std::endl;
      ss << "\t\t\t<info>" << std::get<1>(e) << "</info>" << std::endl;
      ss << "\t\t</Error>" << std::endl;
    }
  }
  for (auto& sh : _shells)
  {
    ss << sh->get_report_xml();
  }
  ss << "\t</Solid>" << std::endl;
  return ss.str();
}


Nef_polyhedron* Solid::get_nef_polyhedron()
{
  if (_nef != NULL)
    return _nef;
  std::vector<Nef_polyhedron> nefs;
  for (auto& sh : this->get_shells())
  {
    //-- convert to an EPEC Polyhedron so that convertion to Nef is possible
    CgalPolyhedronE pe;
    Polyhedron_convert polyhedron_converter(*(sh->get_cgal_polyhedron()));
    pe.delegate(polyhedron_converter);
    Nef_polyhedron onef(pe);
    nefs.push_back(onef);
  }
  Nef_polyhedron* re = new Nef_polyhedron;
  *re += nefs[0];
  for (int i = 1; i < nefs.size(); i++) 
  {
    *re -= nefs[i];
  }
  _nef = re;
  return re;
}


std::set<int> Solid::get_unique_error_codes() {
  std::set<int> errs = Primitive::get_unique_error_codes();
  for (auto& sh : _shells) {
    std::set<int> tmp = sh->get_unique_error_codes();
    errs.insert(tmp.begin(), tmp.end());
  }
  return errs;
}


int Solid::num_ishells()
{
  return (_shells.size() - 1);
}

int Solid::num_faces()
{
  int total = 0;
  for (auto& sh : _shells)
    total += sh->number_faces();
  return total;
}

int Solid::num_vertices()
{
  int total = 0;
  for (auto& sh : _shells)
    total += sh->number_vertices();
  return total;
}


bool Solid::validate_solid_with_nef()
{
  bool isValid = true;
  //-- check orientation of the normals is outwards or inwards
  std::clog << "-----Global orientation of normals" << std::endl;
  int i = 0;
  for (auto& sh : this->get_shells())
  {
    if (check_global_orientation_normals(sh->get_cgal_polyhedron(), i == 0) == false) 
    {
      this->add_error(405, "i", "");
      isValid = false;
    }
    i++;
  }
  if (isValid == false)
    return false;

  if (this->num_ishells() == 0)
    return true;
    
  std::clog << "-----Inspection interactions between the " << (this->num_ishells() + 1) << " shells" << std::endl;
  std::vector<Nef_polyhedron> nefs;
  for (auto& sh : this->get_shells())
  {
    //-- convert to an EPEC Polyhedron so that convertion to Nef is possible
    CgalPolyhedronE pe;
    Polyhedron_convert polyhedron_converter(*(sh->get_cgal_polyhedron()));
    pe.delegate(polyhedron_converter);
    Nef_polyhedron onef(pe);
    nefs.push_back(onef);
  }

  //-- test axiom #1 from the paper, Sect 4.5:
  //-- https://3d.bk.tudelft.nl/hledoux/pdfs/13_cacaie.pdf
  Nef_polyhedron nef;
  for (int i = 1; i < nefs.size(); i++) 
  {
    nef = !nefs[0] * nefs[i];
    if (nef.is_empty() == false)
    {
      nef = nefs[0] * nefs[i];
      if (nef.is_empty() == true)
      {
        std::stringstream msg;
        msg << "Inner shell (#" << i << ") is completely outside the outer shell (#0))";
        this->add_error(403, "i", msg.str());
        isValid = false;
      }
      else
      {
        std::stringstream ss;
        ss << 0 << "--" << i;
        this->add_error(401, ss.str(), "");
        isValid = false; 
      }
    }
    else
    {
      nef = nefs[0] - nefs[i];
      if (nef.number_of_volumes() < 3)
      {
        std::stringstream ss;
        ss << 0 << "--" << i;
        this->add_error(401, ss.str(), "");
        isValid = false; 
      }
    }
  }

  //-- test axiom #2 from the paper 
  nef.clear();
  for (int i = 1; i < nefs.size(); i++) 
  {
    for (int j = (i + 1); j < nefs.size(); j++) 
    {
      //-- 1. are they the same?
      if (nefs[i] == nefs[j])
      {
        std::stringstream ss;
        ss << i << "--" << j;
        this->add_error(402, ss.str(), "");
        isValid = false;
        continue;
      }
      //-- 2. intersection
      nef = nefs[i] * nefs[j];
      if (nef.number_of_volumes() > 1)
      {
        std::stringstream ss;
        ss << i << "--" << j;
        this->add_error(401, ss.str(), "");
        isValid = false;
        continue;
      }
      //-- 3. union
      nef = nefs[i] + nefs[j];
      if (nef.number_of_volumes() < 3)
      {
        std::stringstream ss;
        ss << 0 << "--" << i;
        this->add_error(401, ss.str(), "");
        isValid = false;
        continue;
      }
    }
  }
  //-- test axiom #3 from the paper
  if (isValid == true)
  {
    nef.clear();
    nef += nefs[0];
    int numvol = 2;
    for (int i = 1; i < nefs.size(); i++) 
    {
      nef = nef - nefs[i];
      numvol++;
      if (nef.number_of_volumes() != numvol)
      {
        this->add_error(404, "", "");
        isValid = false;
        break;
      }
    }
  }
  return isValid;
}

} // namespace val3dity
