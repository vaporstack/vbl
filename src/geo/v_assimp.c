//
//  v_assimp.c
//  vbl
//
//  Created by vs on 4/12/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "v_assimp.h"

#include <r4/r4.h>


#ifdef R4_ENABLE_ASSIMP


#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//	todo package this up so not everything knows about assimp

#include <assimp/cimport.h>
#include <assimp/color4.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdlib.h>
extern void* _vbl_internal_rng;

static void _recursive_render(const struct aiScene* sc, const struct aiNode* nd);
static void _recursive_render_random(const struct aiScene* scene, const struct aiNode* nd, double delta);
static void _recursive_render_partial(const struct aiScene* scene, const struct aiNode* nd, double delta);
static void _recursive_render_distance(const struct aiScene* scene, const struct aiNode* nd, double x, double y, double z, double ir, double or);


#ifdef __APPLE__
//	probably can go
//#include <GLUT/glut.h>
#else
//#include <GL/glut.h>
#endif

void v_render_assimp_point_distance(DAssimpRec* rec, double x, double y, double z, double ir, double or)
{
	const struct aiScene* scene = rec->scene.data;
	if ( !scene )
	{
		l_warning("No scene loaded!\n");
		return;
	}
	
	_recursive_render_distance(scene, scene->mRootNode, x, y, z, ir, or);
}
void v_render_assimp(DAssimpRec* rec)
{
	const struct aiScene* scene = rec->scene.data;
	if (!scene )
	{
		printf("ACK scene null\n");
		return;
		
	}
	_recursive_render(scene, scene->mRootNode);
}

void v_render_assimp_randomized(DAssimpRec* rec, double delta)
{
	const struct aiScene* scene = rec->scene.data;
	_recursive_render_random(scene, scene->mRootNode, delta);
}
void v_render_assimp_partial(DAssimpRec* rec, double delta)
{
	const struct aiScene* scene = rec->scene.data;
	_recursive_render_partial(scene, scene->mRootNode, delta);
}

void v_render_assimp_lightbeam(DAssimpRec* rec, double delta)
{
	
}
/*
void apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	ai_real shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE,
&diffuse)) color4_to_float4(&diffuse, c); glMaterialfv(GL_FRONT_AND_BACK,
GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR,
&specular)) color4_to_float4(&specular, c); glMaterialfv(GL_FRONT_AND_BACK,
GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT,
&ambient)) color4_to_float4(&ambient, c); glMaterialfv(GL_FRONT_AND_BACK,
GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE,
&emission)) color4_to_float4(&emission, c); glMaterialfv(GL_FRONT_AND_BACK,
GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess,
&max); if(ret1 == AI_SUCCESS) { max = 1; ret2 = aiGetMaterialFloatArray(mtl,
AI_MATKEY_SHININESS_STRENGTH, &strength, &max); if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess *
strength); else glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl,
AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max)) fill_mode = wireframe ? GL_LINE :
GL_FILL; else fill_mode = GL_FILL; glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED,
&two_sided, &max)) && two_sided) glDisable(GL_CULL_FACE); else
		glEnable(GL_CULL_FACE);
}


void get_bounding_box_for_node (const struct aiNode* nd,
				struct aiVector3D* min,
				struct aiVector3D* max,
				struct aiMatrix4x4* trafo
				){
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;
	
	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);
	
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {
			
			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);
			
			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);
			
			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}
	
	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

void get_bounding_box (struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}


void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}


void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}


 */

void _recursive_render(const struct aiScene* scene, const struct aiNode* nd)
{
	unsigned int       i;
	unsigned int       j;
	unsigned int       n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

#ifdef RPLATFORM_IOS

	printf("ios can't render in thsi way cuz of old gl calls\n");

#else

	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		// apply_material(scene->mMaterials[mesh->mMaterialIndex]);

		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			// glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum		     face_mode;

			switch (face->mNumIndices)
			{
			case 1:
				face_mode = GL_POINTS;
				break;
			case 2:
				face_mode = GL_LINES;
				break;
			case 3:
				face_mode = GL_TRIANGLE_STRIP;
				//face_mode = GL_TRIANGLES;
				break;
			default:
				//face_mode = GL_POLYGON;
				face_mode = GL_LINE_LOOP;
				break;
			}

			face_mode = GL_LINE_LOOP;

			glBegin(face_mode);

			GLfloat* arr =
			    calloc(face->mNumIndices * 3, sizeof(float));

			for (i = 0, j = 0; i < face->mNumIndices; i++, j += 2)
			{
				int index = face->mIndices[i];
				// if(mesh->mColors[0] != NULL)
				//	glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				// if(mesh->mNormals != NULL)
				//	glNormal3fv(&mesh->mNormals[index].x);
				if (i > 0)
				{
					glVertex3fv(&mesh->mVertices[index - 1].x);
				}
				struct aiVector3D vec = mesh->mVertices[index];
				arr[j + 0]	    = vec.x;
				arr[j + 1]	    = vec.y;
				arr[j + 2]	    = vec.z;
				//printf("%f %f %f\n", vec.x, vec.y, vec.z);
				//glVertex3fv(&mesh->mVertices[index].x);

				if (i < face->mNumIndices - 1)
				{
					glVertex3fv(&mesh->mVertices[index + 1].x);
				}
			}

			//glVertexPointer(3, GL_FLOAT, 0, arr);
			//glDrawArrays(GL_LINES, 0,  face->mNumIndices);

			glEnd();

			glPushMatrix();
			//glTranslated(16, 0, 0);
			// glVertexPointer(3, GL_FLOAT, 0, arr);
			// glDrawArrays(GL_LINE_STRIP, 0, face->mNumIndices);
			glPopMatrix();
			free(arr);
		}
	}

	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		_recursive_render(scene, nd->mChildren[n]);
	}

	glPopMatrix();
#endif
}

void _recursive_render_partial(const struct aiScene* scene, const struct aiNode* nd, double delta)
{
	unsigned int       i;
	unsigned int       j;
	unsigned int       n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;
	
	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
#ifdef RPLATFORM_IOS
	printf("ios can't render in thsi way cuz of old gl calls\n");
	
#else
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		
		// apply_material(scene->mMaterials[mesh->mMaterialIndex]);
		
		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			// glEnable(GL_LIGHTING);
		}
		int num = mesh->mNumFaces;
		
		int amt_to_render = delta * num;
		//printf("Rendering %d faces\n", amt_to_render);
		for (t = 0; t < amt_to_render; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum		     face_mode;
			
			switch (face->mNumIndices)
			{
				case 1:
					face_mode = GL_POINTS;
					break;
				case 2:
					face_mode = GL_LINES;
					break;
				case 3:
					face_mode = GL_TRIANGLE_STRIP;
					//face_mode = GL_TRIANGLES;
					break;
				default:
					//face_mode = GL_POLYGON;
					face_mode = GL_LINE_LOOP;
					break;
			}
			/*
			 face_mode = GL_TRIANGLE_FAN;
			 face_mode = GL_TRIANGLE_STRIP;
			 //face_mode = GL_LINE_STRIP;
			 face_mode = GL_POLYGON;
			 face_mode = GL_LINES;
			 */
			face_mode = GL_LINES;
			
			//	TODO: figure out how to untangle this mess. Using random
			//	code in library functions obviously means that software
			//	level pausing will break - probably
			//	vbl needs its own library-wide RNG reference that can
			//	be set by the calling program?
			//double throw = r_rand_double(NULL);
			//if ( throw < delta )
			//{
				glBegin(face_mode);
				
				GLfloat* arr =
				calloc(face->mNumIndices * 3, sizeof(float));
				
				for (i = 0, j = 0; i < face->mNumIndices; i++, j += 2)
				{
					int index = face->mIndices[i];
					// if(mesh->mColors[0] != NULL)
					//	glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					// if(mesh->mNormals != NULL)
					//	glNormal3fv(&mesh->mNormals[index].x);
					if (i > 0)
					{
						glVertex3fv(&mesh->mVertices[index - 1].x);
					}
					struct aiVector3D vec = mesh->mVertices[index];
					arr[j + 0]	    = vec.x;
					arr[j + 1]	    = vec.y;
					arr[j + 2]	    = vec.z;
					//printf("%f %f %f\n", vec.x, vec.y, vec.z);
					//glVertex3fv(&mesh->mVertices[index].x);
					
					if (i < face->mNumIndices - 1)
					{
						glVertex3fv(&mesh->mVertices[index + 1].x);
					}
				}
				
				//glVertexPointer(3, GL_FLOAT, 0, arr);
				//glDrawArrays(GL_LINES, 0,  face->mNumIndices);
				
				glEnd();
				
				glPushMatrix();
				//glTranslated(16, 0, 0);
				// glVertexPointer(3, GL_FLOAT, 0, arr);
				// glDrawArrays(GL_LINE_STRIP, 0, face->mNumIndices);
				glPopMatrix();
				free(arr);
			//}
		}
	}
	
	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		_recursive_render_partial(scene, nd->mChildren[n], delta);
	}
	
	glPopMatrix();
#endif
}

void _recursive_render_random(const struct aiScene* scene, const struct aiNode* nd, double delta)
{
	unsigned int       i;
	unsigned int       j;
	unsigned int       n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;
	
	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
#ifdef RPLATFORM_IOS
	printf("ios can't render in thsi way cuz of old gl calls\n");
	
#else
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		
		// apply_material(scene->mMaterials[mesh->mMaterialIndex]);
		
		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			// glEnable(GL_LIGHTING);
		}
		
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum		     face_mode;
			
			switch (face->mNumIndices)
			{
				case 1:
					face_mode = GL_POINTS;
					break;
				case 2:
					face_mode = GL_LINES;
					break;
				case 3:
					face_mode = GL_TRIANGLE_STRIP;
					//face_mode = GL_TRIANGLES;
					break;
				default:
					//face_mode = GL_POLYGON;
					face_mode = GL_LINE_LOOP;
					break;
			}
			/*
			 face_mode = GL_TRIANGLE_FAN;
			 face_mode = GL_TRIANGLE_STRIP;
			 //face_mode = GL_LINE_STRIP;
			 face_mode = GL_POLYGON;
			 face_mode = GL_LINES;
			 */
			face_mode = GL_LINE_LOOP;
			
			//	TODO: figure out how to untangle this mess. Using random
			//	code in library functions obviously means that software
			//	level pausing will break - probably
			//	vbl needs its own library-wide RNG reference that can
			//	be set by the calling program?
			double throw = r_rand_double(_vbl_internal_rng);
			if ( throw < delta )
			{
				glBegin(face_mode);
				
				GLfloat* arr =
				calloc(face->mNumIndices * 3, sizeof(float));
				
				for (i = 0, j = 0; i < face->mNumIndices; i++, j += 2)
				{
					int index = face->mIndices[i];
					// if(mesh->mColors[0] != NULL)
					//	glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					// if(mesh->mNormals != NULL)
					//	glNormal3fv(&mesh->mNormals[index].x);
					if (i > 0)
					{
						glVertex3fv(&mesh->mVertices[index - 1].x);
					}
					struct aiVector3D vec = mesh->mVertices[index];
					arr[j + 0]	    = vec.x;
					arr[j + 1]	    = vec.y;
					arr[j + 2]	    = vec.z;
					//printf("%f %f %f\n", vec.x, vec.y, vec.z);
					//glVertex3fv(&mesh->mVertices[index].x);
					
					if (i < face->mNumIndices - 1)
					{
						glVertex3fv(&mesh->mVertices[index + 1].x);
					}
				}
				
				//glVertexPointer(3, GL_FLOAT, 0, arr);
				//glDrawArrays(GL_LINES, 0,  face->mNumIndices);
				
				glEnd();
				
				glPushMatrix();
				//glTranslated(16, 0, 0);
				// glVertexPointer(3, GL_FLOAT, 0, arr);
				// glDrawArrays(GL_LINE_STRIP, 0, face->mNumIndices);
				glPopMatrix();
				free(arr);
			}
		}
	}
	
	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		_recursive_render_random(scene, nd->mChildren[n], delta);
	}
	
	glPopMatrix();
#endif
}

void _recursive_render_distance(const struct aiScene* scene, const struct aiNode* nd, double x, double y, double z, double ir, double or)
{
	unsigned int       i;
	unsigned int       j;
	unsigned int       n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;
	
	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
#ifdef RPLATFORM_IOS
	printf("ios can't render in thsi way cuz of old gl calls\n");
	
#else
	
	vec3_t point = vec3_create(NULL);
	vec3_t test = vec3_create(NULL);
	
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		
		// apply_material(scene->mMaterials[mesh->mMaterialIndex]);
		
		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			// glEnable(GL_LIGHTING);
		}
		
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum		     face_mode;
			
			switch (face->mNumIndices)
			{
				case 1:
					face_mode = GL_POINTS;
					break;
				case 2:
					face_mode = GL_LINES;
					break;
				case 3:
					face_mode = GL_TRIANGLE_STRIP;
					//face_mode = GL_TRIANGLES;
					break;
				default:
					//face_mode = GL_POLYGON;
					face_mode = GL_LINE_LOOP;
					break;
			}
			/*
			 face_mode = GL_TRIANGLE_FAN;
			 face_mode = GL_TRIANGLE_STRIP;
			 //face_mode = GL_LINE_STRIP;
			 face_mode = GL_POLYGON;
			 face_mode = GL_LINES;
			 */
			face_mode = GL_LINE_LOOP;
			
			//	TODO: figure out how to untangle this mess. Using random
			//	code in library functions obviously means that software
			//	level pausing will break - probably
			//	vbl needs its own library-wide RNG reference that can
			//	be set by the calling program?
			//double throw = r_rand_double(_vbl_internal_rng);
			//if ( throw < delta )
			//{
			
			
			point[0] = x;
			point[1] = y;
			point[2] = z;
		
			glBegin(face_mode);
			
			//GLfloat* arr =
			//calloc(face->mNumIndices * 3, sizeof(float));
			bool can_draw = false;
			for (i = 0, j = 0; i < face->mNumIndices; i++, j += 2)
			{
				int index = face->mIndices[i];
				if (i > 0)
				{
					
					//glVertex3fv(&mesh->mVertices[index - 1].x);
				}
				struct aiVector3D vec = mesh->mVertices[index];
			
				test[0] = vec.x;
				test[1] = vec.y;
				test[2] = vec.z;
				char buf[256];
				vec3_str(point, buf);
				
				//printf("%s\n", buf);
				vec3_str(test, buf);
				//printf("%s\n", buf);
				
				double d = vec3_dist(point, test);
				if ( d > ir && d < or )
				{
					can_draw = true;
				}

				if (i < face->mNumIndices - 1)
				{
					//glVertex3fv(&mesh->mVertices[index + 1].x);
				}
			}
			if( can_draw)
			{
				for (i = 0, j = 0; i < face->mNumIndices; i++, j += 2)
				{
					int index = face->mIndices[i];
					if (i > 0)
					{
						
						glVertex3fv(&mesh->mVertices[index - 1].x);
					}
					//struct aiVector3D vec = mesh->mVertices[index];
					
					if (i < face->mNumIndices - 1)
					{
						glVertex3fv(&mesh->mVertices[index + 1].x);
					}
				}
				
		
			}
			glEnd();

		}
	}
	
	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		_recursive_render_distance(scene, nd->mChildren[n], x, y, z, ir, or);
	}
	
	glPopMatrix();
#endif
	
	
	free(point);
	free(test);
}


#endif
