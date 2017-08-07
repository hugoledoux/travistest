//
//  Building.h
//  val3dity
//
//  Created by Hugo Ledoux on 27/10/16.
//
//

#ifndef Building_h
#define Building_h

#include "definitions.h"
#include "Primitive.h"
#include <string>
#include <vector>

namespace val3dity
{

class BuildingPart
{
public:
  BuildingPart  (std::string id = "");
  ~BuildingPart ();

  bool          validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap = -1);
  bool          is_valid();
  bool          is_empty();
  std::string   get_report_xml();

  std::string   get_id();
  void          set_id(std::string id);
  int           get_number_primitives();
  int           get_number_solids();
  int           get_number_compositesolids();
  int           get_number_multisurfaces();


  void          add_primitive(Primitive* p);

  std::vector<Primitive*>&   get_primitives();

protected:
  std::string                _id;
  int                        _is_valid;
  std::vector<Primitive*>    _lsPrimitives;
};


class Building
{
public:
  Building  (std::string id = "");
  ~Building ();
  
  bool          validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap = -1);
  bool          is_valid();
  bool          is_empty();
  void          add_error(int code, std::string whichgeoms, std::string info);
  std::string   get_report_xml();
  
  std::string   get_id();
  void          set_id(std::string id);

  void          add_primitive(Primitive* p);
  std::set<int> get_unique_error_codes();
  int           get_number_primitives();
  int           get_number_solids();
  int           get_number_compositesolids();
  int           get_number_multisurfaces();
  void          get_min_bbox(double& x, double& y);
  void          translate_vertices(double minx, double miny);
  
  void          add_buildingpart(BuildingPart* bp);
  int           get_number_buildingparts();
  bool          has_parts();
  
protected:
  std::string                _id;
  int                        _is_valid;
  std::vector<Primitive*>    _lsPrimitives;
  std::vector<BuildingPart*> _lsBP;

  std::map<int, std::vector< std::tuple< std::string, std::string > > > _errors;
};

} // namespace val3dity

#endif /* Building_h */
