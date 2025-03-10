#ifndef _SOLIDOBJECT_H_
#define _SOLIDOBJECT_H_

#include "zfix.h"
#include "base/zc_alleg.h"

class solid_object;

bool collide_object(solid_object const* obj);
bool collide_object(int32_t tx, int32_t ty, int32_t tw, int32_t th, solid_object const* ign = nullptr);
void draw_solid_objects(BITMAP *dest, int32_t x, int32_t y, int32_t col);
void setCurObject(solid_object* obj);

class solid_object
{
public:
	zfix x, y, vx, vy;
	zfix old_x, old_y, old_x2, old_y2;
	int32_t hxsz,hysz,hxofs,hyofs;
	int32_t sxofs,syofs,sxsz_ofs,sysz_ofs;
	int32_t solidflags, dir;
	bool switch_hooked;
	
	solid_object();
	~solid_object();
	virtual void copy(solid_object const& other);
	solid_object(solid_object const& other);
	solid_object& operator=(solid_object const& other);
	
	virtual bool setSolid(bool set);
	virtual bool getSolid() const;
	virtual void updateSolid();
	void setTempNonsolid(bool set);
	bool getTempNonsolid() const;
	
	virtual bool collide(solid_object const* other) const;
	virtual bool collide(int32_t tx, int32_t ty, int32_t tw, int32_t th) const;
	
	void draw(BITMAP *dest, int32_t tx, int32_t ty, int32_t col);
	virtual void solid_update(bool push = true);
	virtual void solid_push(solid_object* pusher);
	//Overload to do damage to Hero on pushing them
	virtual void doContactDamage(int32_t hdir){}
	//Overload to give 'riding sideview platform' behaviors
	virtual bool sideview_mode() const {return false;}
protected:
	bool solid;
	bool ignore_solid_temp;
	void solid_push_int(solid_object const* obj, zfix& dx, zfix& dy, int32_t& hdir);
	int32_t push_dir() const;
	
	virtual bool is_unpushable() const {return false;}
private:
	bool in_solid_arr;
};



#endif

