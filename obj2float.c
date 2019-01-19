/* -*- mode: c; c-basic-offset: 4; -*- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTEX	100000
#define MAX_NORMAL	100000
#define MAX_UV		100000

struct vertex
{
    float x, y, z;
};
struct normal
{
    float x, y, z;
};
struct uv
{
    float u, v;
};
struct all
{
    struct vertex v;
    struct uv t;
    struct normal n;
};

void showall(struct all * all)
{
    printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
	   all->v.x, all->v.y, all->v.z,
	   all->t.u, all->t.v,
	   all->n.x, all->n.y, all->n.z);
}

int main(int argc, char *argv[])
{
    char s[8];
    float x, y, z;
    unsigned int v[3], u[3], n[3];
    unsigned int vi = 1, ni = 1, ti = 1; // obj idx start from 1
    int res, matches;
    FILE *f, *fo;
    struct vertex * vertex;
    struct normal * normal;
    struct uv * uv;
    struct all all;
	
    if (argc < 3)
    {
	return 0;
    }

    f = fopen(argv[1], "r");
    if ( !f )
    {
	perror("open");
	return 1;
    }

    fo = fopen(argv[2], "w");
    if ( !fo )
    {
	perror("open");
	return 1;
    }

    vertex = malloc(MAX_VERTEX * sizeof(struct vertex));
    normal = malloc(MAX_NORMAL * sizeof(struct normal));
    uv = malloc(MAX_UV * sizeof(struct uv));
    if (!vertex || !normal || !uv)
    {
	fprintf(stderr, "out of memory\n");
	return 1;
    }

    while (1)
    {
	res = fscanf(f, "%3s", s);
		
	if (res == EOF)
	    break;
		
	if ( strcmp( s, "v" ) == 0 )
	{
	    if (vi >= MAX_VERTEX)
	    {
		fprintf(stderr, "vertex limit\n");
		return 1;
	    }
	    fscanf(f, "%f %f %f\n", &x, &y, &z );
	    vertex[vi].x = x;
	    vertex[vi].y = y;
	    vertex[vi].z = z;
	    ++vi;
	}
	else if ( strcmp( s, "vt" ) == 0 )
	{
	    if (ti >= MAX_UV)
	    {
		fprintf(stderr, "uv limit\n");
		return 1;
	    }
	    fscanf(f, "%f %f\n", &x, &y );
	    uv[ti].u = x;
	    uv[ti].v = y;
	    ++ti;
	}
	else if ( strcmp( s, "vn" ) == 0 )
	{
	    if (ni >= MAX_NORMAL)
	    {
		fprintf(stderr, "normal limit\n");
		return 1;
	    }
	    fscanf(f, "%f %f %f\n", &x, &y, &z );
	    normal[ni].x = x;
	    normal[ni].y = y;
	    normal[ni].z = z;
	    ++ni;
	}
	else if ( strcmp( s, "f" ) == 0 )
	{
	    matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			     &v[0], &u[0], &n[0],
			     &v[1], &u[1], &n[1],
			     &v[2], &u[2], &n[2]);
	    if (matches != 9){
		fprintf(stderr, "File can't be read by our simple parser"
			"( Try exporting with other options\n");
		return 1;
	    }

	    all.v = vertex[v[0]]; all.t = uv[u[0]]; all.n = normal[n[0]];
	    showall(&all);
	    fwrite(&all, sizeof(struct all), 1, fo);

	    all.v = vertex[v[1]]; all.t = uv[u[1]]; all.n = normal[n[1]];
	    showall(&all);
	    fwrite(&all, sizeof(struct all), 1, fo);

	    all.v = vertex[v[2]]; all.t = uv[u[2]]; all.n = normal[n[2]];
	    showall(&all);
	    fwrite(&all, sizeof(struct all), 1, fo);
	}
    }
    
    fclose(fo);
    fclose(f);
	
    return 0;
}

