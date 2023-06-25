// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	// TODO: implement the Rodriguez formula here
	vec3 n_axis = glm::normalize(axis);
	float radians = glm::radians(degrees);
	float cos_theta = glm::cos(radians);
	float sin_theta = glm::sin(radians);
	mat3 identity{vec3(1,0,0), vec3(0,1,0), vec3(0,0,1)};
	mat3 a_dot_a_trans = { vec3(n_axis.x* n_axis.x, n_axis.x* n_axis.y, n_axis.x* n_axis.z),
					 	  vec3(n_axis.x* n_axis.y, n_axis.y* n_axis.y, n_axis.y* n_axis.z),
				    	  vec3(n_axis.x* n_axis.z, n_axis.y* n_axis.z, n_axis.z* n_axis.z)
				   		};

	mat3 dual_matrix = { vec3(0, -n_axis.z, n_axis.y),
						 vec3(n_axis.z, 0, -n_axis.x),
						 vec3(-n_axis.y, n_axis.x, 0)
					   };

	return cos_theta * identity + float(1.0 - cos_theta) * a_dot_a_trans + sin_theta * dual_matrix;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  vec3 rot_vector = up;

	mat3 rot_matrix = rotate(-degrees, rot_vector);
	eye = rot_matrix * eye ;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  vec3 rot_vector = glm::normalize(glm::cross(-eye, up));
	mat3 rot_matrix = rotate(degrees, rot_vector);
	eye = rot_matrix * eye;
	up = glm::normalize(rot_matrix * up);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  vec3 z = glm::normalize(-eye);
	vec3 x = glm::normalize(glm::cross(z, up));
	vec3 y = glm::cross(x, z);

	vec3 z_inverse = -z; 

	mat4 lookAt_matrix = {
							vec4(x.x, x.y, x.z, -glm::dot(x, eye)),
							vec4(y.x, y.y, y.z, -glm::dot(y, eye)),
							vec4(z_inverse.x, z_inverse.y, z_inverse.z, -glm::dot(z_inverse, eye)),
							vec4(0, 0, 0, 1)
						 }; 
	return glm::transpose(lookAt_matrix);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float tan_theta = glm::tan(fovy * 0.5);
	float delta_far = zFar - zNear;

  	mat4 ret{vec4(1 / (aspect * tan_theta), 0, 0, 0),
  		   	 vec4(0, 1.0f / tan_theta, 0, 0),
		   	 vec4(0, 0, -(zFar + zNear)/delta_far, (-2* zFar * zNear)/delta_far),
		   	 vec4(0,0,-1,0)};

  	return glm::transpose(ret);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret{vec4(sx,0,0,0),
  		   vec4(0,sy,0,0),
		   vec4(0,0,sz,0),
		   vec4(0,0,0, 1)};
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret{vec4(1,0,0,0),
  		   vec4(0,1,0,0),
		   vec4(0,0,1,0),
		   vec4(tx,ty,tz,1)};
 
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
