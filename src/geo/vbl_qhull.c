//
//  vbl_qhull.c
//  r4
//
//  Created by vs on 7/17/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_qhull.h"

//#define qh_QHimport
//#include "libqhull/qhull_a.h"
//#include <deps/qhull/libqhull.h>

#include <drw/drw.h>
#include <r4/src/core/r_random.h>
#ifdef R4_ENABLE_QHULL

//todo: abstract this data type and dont make vbl know about
//	rline maybe?
//	that might be a stupid statement

//	from user_r.c (libqhull

/*<html><pre>  -<a                             href="qh-user_r.htm"
 >-------------------------------</a><a name="TOP">-</a>

 user.c
 user redefinable functions

 see user2_r.c for qh_fprintf, qh_malloc, qh_free

 see README.txt  see COPYING.txt for copyright information.

 see libqhull_r.h for data structures, macros, and user-callable functions.

 see user_eg.c, user_eg2.c, and unix.c for examples.

 see user.h for user-definable constants

 use qh_NOmem in mem_r.h to turn off memory management
 use qh_NOmerge in user.h to turn off facet merging
 set qh_KEEPstatistics in user.h to 0 to turn off statistics

 This is unsupported software.  You're welcome to make changes,
 but you're on your own if something goes wrong.  Use 'Tc' to
 check frequently.  Usually qhull will report an error if
 a data structure becomes inconsistent.  If so, it also reports
 the last point added to the hull, e.g., 102.  You can then trace
 the execution of qhull with "T4P102".

 Please report any errors that you fix to qhull@qhull.org

 Qhull-template is a template for calling qhull from within your application

 if you recompile and load this module, then user.o will not be loaded
 from qhull.a

 you can add additional quick allocation sizes in qh_user_memsizes

 if the other functions here are redefined to not use qh_print...,
 then io.o will not be loaded from qhull.a.  See user_eg_r.c for an
 example.  We recommend keeping io.o for the extra debugging
 information it supplies.
 */

#include <deps/libqhull_r/qhull_ra.h>

#include <stdarg.h>

/*-<a                             href="qh-user_r.htm#TOC"
 >-------------------------------</a><a name="qhull_template">-</a>

 Qhull-template
 Template for calling qhull from inside your program

 returns:
 exit code(see qh_ERR... in libqhull_r.h)
 all memory freed

 notes:
 This can be called any number of times.

 */
#if 0
{
	int dim;                  /* dimension of points */
	int numpoints;            /* number of points */
	coordT *points;           /* array of coordinates for each point */
	boolT ismalloc;           /* True if qhull should free points in qh_freeqhull() or reallocation */
	char flags[]= "qhull Tv"; /* option flags for qhull, see qh_opt.htm */
	FILE *outfile= stdout;    /* output from qh_produce_output(qh)
				   use NULL to skip qh_produce_output(qh) */
	FILE *errfile= stderr;    /* error messages from qhull code */
	int exitcode;             /* 0 if no error from qhull */
	facetT *facet;            /* set by FORALLfacets */
	int curlong, totlong;     /* memory remaining after qh_memfreeshort */

	qhT qh_qh;                /* Qhull's data structure.  First argument of most calls */
	qhT *qh= &qh_qh;          /* Alternatively -- qhT *qh= (qhT*)malloc(sizeof(qhT)) */

	QHULL_LIB_CHECK /* Check for compatible library */

	qh_zero(qh, errfile);

	/* initialize dim, numpoints, points[], ismalloc here */
	exitcode= qh_new_qhull(qh, dim, numpoints, points, ismalloc,
			       flags, outfile, errfile);
	if (!exitcode) {                  /* if no error */
		/* 'qh->facet_list' contains the convex hull */
		FORALLfacets {
			/* ... your code ... */
		}
	}
	qh_freeqhull(qh, !qh_ALL);
	qh_memfreeshort(qh, &curlong, &totlong);
	if (curlong || totlong)
		qh_fprintf(qh, errfile, 7068, "qhull internal warning (main): did not free %d bytes of long memory(%d pieces)\n", totlong, curlong);
		}
#endif

/*-<a                             href="qh-user_r.htm#TOC"
 >-------------------------------</a><a name="new_qhull">-</a>

 qh_new_qhull(qh, dim, numpoints, points, ismalloc, qhull_cmd, outfile, errfile )
 Run qhull and return results in qh.
 Returns exitcode (0 if no errors).
 Before first call, either call qh_zero(qh, errfile), or set qh to all zero.

 notes:
 do not modify points until finished with results.
 The qhull data structure contains pointers into the points array.
 do not call qhull functions before qh_new_qhull().
 The qhull data structure is not initialized until qh_new_qhull().
 do not call qh_init_A (global_r.c)

 Default errfile is stderr, outfile may be null
 qhull_cmd must start with "qhull "
 projects points to a new point array for Delaunay triangulations ('d' and 'v')
 transforms points into a new point array for halfspace intersection ('H')

 see:
 Qhull-template at the beginning of this file.
 An example of using qh_new_qhull is user_eg_r.c
 */

int qh_new_qhull(qhT* qh, int dim, int numpoints, coordT* points, boolT ismalloc,
		 char* qhull_cmd, FILE* outfile, FILE* errfile)
{
	/* gcc may issue a "might be clobbered" warning for dim, points, and ismalloc [-Wclobbered].
	 These parameters are not referenced after a longjmp() and hence not clobbered.
	 See http://stackoverflow.com/questions/7721854/what-sense-do-these-clobbered-variable-warnings-make */
	int     exitcode, hulldim;
	boolT   new_ismalloc;
	coordT* new_points;

	if (!errfile)
	{
		errfile = stderr;
	}
	if (!qh->qhmem.ferr)
	{
		qh_meminit(qh, errfile);
	}
	else
	{
		qh_memcheck(qh);
	}
	if (strncmp(qhull_cmd, "qhull ", (size_t)6))
	{
		qh_fprintf(qh, errfile, 6186, "qhull error (qh_new_qhull): start qhull_cmd argument with \"qhull \"\n");
		return qh_ERRinput;
	}
	qh_initqhull_start(qh, NULL, outfile, errfile);
	trace1((qh, qh->ferr, 1044, "qh_new_qhull: build new Qhull for %d %d-d points with %s\n", numpoints, dim, qhull_cmd));
	exitcode = setjmp(qh->errexit);
	if (!exitcode)
	{
		qh->NOerrexit = False;
		qh_initflags(qh, qhull_cmd);
		if (qh->DELAUNAY)
			qh->PROJECTdelaunay = True;
		if (qh->HALFspace)
		{
			/* points is an array of halfspaces,
			 the last coordinate of each halfspace is its offset */
			hulldim = dim - 1;
			qh_setfeasible(qh, hulldim);
			new_points   = qh_sethalfspace_all(qh, dim, numpoints, points, qh->feasible_point);
			new_ismalloc = True;
			if (ismalloc)
				qh_free(points);
		}
		else
		{
			hulldim      = dim;
			new_points   = points;
			new_ismalloc = ismalloc;
		}
		qh_init_B(qh, new_points, numpoints, hulldim, new_ismalloc);
		qh_qhull(qh);
		qh_check_output(qh);
		if (outfile)
		{
			qh_produce_output(qh);
		}
		else
		{
			qh_prepare_output(qh);
		}
		if (qh->VERIFYoutput && !qh->STOPpoint && !qh->STOPcone)
			qh_check_points(qh);
	}
	qh->NOerrexit = True;
	return exitcode;
} /* new_qhull */

/*-<a                             href="qh-user_r.htm#TOC"
 >-------------------------------</a><a name="errexit">-</a>

 qh_errexit(qh, exitcode, facet, ridge )
 report and exit from an error
 report facet and ridge if non-NULL
 reports useful information such as last point processed
 set qh.FORCEoutput to print neighborhood of facet

 see:
 qh_errexit2() in libqhull_r.c for printing 2 facets

 design:
 check for error within error processing
 compute qh.hulltime
 print facet and ridge (if any)
 report commandString, options, qh.furthest_id
 print summary and statistics (including precision statistics)
 if qh_ERRsingular
 print help text for singular data set
 exit program via long jump (if defined) or exit()
 */
void qh_errexit(qhT* qh, int exitcode, facetT* facet, ridgeT* ridge)
{

	if (qh->ERREXITcalled)
	{
		qh_fprintf(qh, qh->ferr, 8126, "\nqhull error while processing previous error.  Exit program\n");
		qh_exit(qh_ERRqhull);
	}
	qh->ERREXITcalled = True;
	if (!qh->QHULLfinished)
		qh->hulltime = qh_CPUclock - qh->hulltime;
	qh_errprint(qh, "ERRONEOUS", facet, NULL, ridge, NULL);
	qh_fprintf(qh, qh->ferr, 8127, "\nWhile executing: %s | %s\n", qh->rbox_command, qh->qhull_command);
	qh_fprintf(qh, qh->ferr, 8128, "Options selected for Qhull %s:\n%s\n", qh_version, qh->qhull_options);
	if (qh->furthest_id >= 0)
	{
		qh_fprintf(qh, qh->ferr, 8129, "Last point added to hull was p%d.", qh->furthest_id);
		if (zzval_(Ztotmerge))
			qh_fprintf(qh, qh->ferr, 8130, "  Last merge was #%d.", zzval_(Ztotmerge));
		if (qh->QHULLfinished)
			qh_fprintf(qh, qh->ferr, 8131, "\nQhull has finished constructing the hull.");
		else if (qh->POSTmerging)
			qh_fprintf(qh, qh->ferr, 8132, "\nQhull has started post-merging.");
		qh_fprintf(qh, qh->ferr, 8133, "\n");
	}
	if (qh->FORCEoutput && (qh->QHULLfinished || (!facet && !ridge)))
		qh_produce_output(qh);
	else if (exitcode != qh_ERRinput)
	{
		if (exitcode != qh_ERRsingular && zzval_(Zsetplane) > qh->hull_dim + 1)
		{
			qh_fprintf(qh, qh->ferr, 8134, "\nAt error exit:\n");
			qh_printsummary(qh, qh->ferr);
			if (qh->PRINTstatistics)
			{
				qh_collectstatistics(qh);
				qh_printstatistics(qh, qh->ferr, "at error exit");
				qh_memstatistics(qh, qh->ferr);
			}
		}
		if (qh->PRINTprecision)
			qh_printstats(qh, qh->ferr, qh->qhstat.precision, NULL);
	}
	if (!exitcode)
		exitcode = qh_ERRqhull;
	else if (exitcode == qh_ERRsingular)
		qh_printhelp_singular(qh, qh->ferr);
	else if (exitcode == qh_ERRprec && !qh->PREmerge)
		qh_printhelp_degenerate(qh, qh->ferr);
	if (qh->NOerrexit)
	{
		qh_fprintf(qh, qh->ferr, 6187, "qhull error while ending program, or qh->NOerrexit not cleared after setjmp(). Exit program with error.\n");
		qh_exit(qh_ERRqhull);
	}
	qh->ERREXITcalled = False;
	qh->NOerrexit     = True;
	qh->ALLOWrestart  = False; /* longjmp will undo qh_build_withrestart */
	longjmp(qh->errexit, exitcode);
} /* errexit */

/*-<a                             href="qh-user_r.htm#TOC"
 >-------------------------------</a><a name="errprint">-</a>

 qh_errprint(qh, fp, string, atfacet, otherfacet, atridge, atvertex )
 prints out the information of facets and ridges to fp
 also prints neighbors and geomview output

 notes:
 except for string, any parameter may be NULL
 */
void qh_errprint(qhT* qh, const char* string, facetT* atfacet, facetT* otherfacet, ridgeT* atridge, vertexT* atvertex)
{
	int i;

	if (atfacet)
	{
		qh_fprintf(qh, qh->ferr, 8135, "%s FACET:\n", string);
		qh_printfacet(qh, qh->ferr, atfacet);
	}
	if (otherfacet)
	{
		qh_fprintf(qh, qh->ferr, 8136, "%s OTHER FACET:\n", string);
		qh_printfacet(qh, qh->ferr, otherfacet);
	}
	if (atridge)
	{
		qh_fprintf(qh, qh->ferr, 8137, "%s RIDGE:\n", string);
		qh_printridge(qh, qh->ferr, atridge);
		if (atridge->top && atridge->top != atfacet && atridge->top != otherfacet)
			qh_printfacet(qh, qh->ferr, atridge->top);
		if (atridge->bottom && atridge->bottom != atfacet && atridge->bottom != otherfacet)
			qh_printfacet(qh, qh->ferr, atridge->bottom);
		if (!atfacet)
			atfacet = atridge->top;
		if (!otherfacet)
			otherfacet = otherfacet_(atridge, atfacet);
	}
	if (atvertex)
	{
		qh_fprintf(qh, qh->ferr, 8138, "%s VERTEX:\n", string);
		qh_printvertex(qh, qh->ferr, atvertex);
	}
	if (qh->fout && qh->FORCEoutput && atfacet && !qh->QHULLfinished && !qh->IStracing)
	{
		qh_fprintf(qh, qh->ferr, 8139, "ERRONEOUS and NEIGHBORING FACETS to output\n");
		for (i = 0; i < qh_PRINTEND; i++) /* use fout for geomview output */
			qh_printneighborhood(qh, qh->fout, qh->PRINTout[i], atfacet, otherfacet,
					     !qh_ALL);
	}
} /* errprint */

/*-<a                             href="qh-user_r.htm#TOC"
 >-------------------------------</a><a name="printfacetlist">-</a>

 qh_printfacetlist(qh, fp, facetlist, facets, printall )
 print all fields for a facet list and/or set of facets to fp
 if !printall,
 only prints good facets

 notes:
 also prints all vertices
 */
void qh_printfacetlist(qhT* qh, facetT* facetlist, setT* facets, boolT printall)
{
	facetT *facet, **facetp;

	qh_printbegin(qh, qh->ferr, qh_PRINTfacets, facetlist, facets, printall);
	FORALLfacet_(facetlist)
	    qh_printafacet(qh, qh->ferr, qh_PRINTfacets, facet, printall);
	FOREACHfacet_(facets)
	    qh_printafacet(qh, qh->ferr, qh_PRINTfacets, facet, printall);
	qh_printend(qh, qh->ferr, qh_PRINTfacets, facetlist, facets, printall);
} /* printfacetlist */

/*-<a                             href="qh-io_r.htm#TOC"
 >-------------------------------</a><a name="printhelp_degenerate">-</a>

 qh_printhelp_degenerate(qh, fp )
 prints descriptive message for precision error

 notes:
 no message if qh_QUICKhelp
 */
void qh_printhelp_degenerate(qhT* qh, FILE* fp)
{

	if (qh->MERGEexact || qh->PREmerge || qh->JOGGLEmax < REALmax / 2)
		qh_fprintf(qh, fp, 9368, "\n\
			   A Qhull error has occurred.  Qhull should have corrected the above\n\
			   precision error.  Please send the input and all of the output to\n\
			   qhull_bug@qhull.org\n");
	else if (!qh_QUICKhelp)
	{
		qh_fprintf(qh, fp, 9369, "\n\
			   Precision problems were detected during construction of the convex hull.\n\
			   This occurs because convex hull algorithms assume that calculations are\n\
			   exact, but floating-point arithmetic has roundoff errors.\n\
			   \n\
			   To correct for precision problems, do not use 'Q0'.  By default, Qhull\n\
			   selects 'C-0' or 'Qx' and merges non-convex facets.  With option 'QJ',\n\
			   Qhull joggles the input to prevent precision problems.  See \"Imprecision\n\
			   in Qhull\" (qh-impre.htm).\n\
			   \n\
			   If you use 'Q0', the output may include\n\
			   coplanar ridges, concave ridges, and flipped facets.  In 4-d and higher,\n\
			   Qhull may produce a ridge with four neighbors or two facets with the same \n\
			   vertices.  Qhull reports these events when they occur.  It stops when a\n\
			   concave ridge, flipped facet, or duplicate facet occurs.\n");
#if REALfloat
		qh_fprintf(qh, fp, 9370, "\
			   \n\
			   Qhull is currently using single precision arithmetic.  The following\n\
			   will probably remove the precision problems:\n\
			   - recompile qhull for realT precision(#define REALfloat 0 in user.h).\n");
#endif
		if (qh->DELAUNAY && !qh->SCALElast && qh->MAXabs_coord > 1e4)
			qh_fprintf(qh, fp, 9371, "\
				   \n\
				   When computing the Delaunay triangulation of coordinates > 1.0,\n\
				   - use 'Qbb' to scale the last coordinate to [0,m] (max previous coordinate)\n");
		if (qh->DELAUNAY && !qh->ATinfinity)
			qh_fprintf(qh, fp, 9372, "\
				   When computing the Delaunay triangulation:\n\
				   - use 'Qz' to add a point at-infinity.  This reduces precision problems.\n");

		qh_fprintf(qh, fp, 9373, "\
			   \n\
			   If you need triangular output:\n\
			   - use option 'Qt' to triangulate the output\n\
			   - use option 'QJ' to joggle the input points and remove precision errors\n\
			   - use option 'Ft'.  It triangulates non-simplicial facets with added points.\n\
			   \n\
			   If you must use 'Q0',\n\
			   try one or more of the following options.  They can not guarantee an output.\n\
			   - use 'QbB' to scale the input to a cube.\n\
			   - use 'Po' to produce output and prevent partitioning for flipped facets\n\
			   - use 'V0' to set min. distance to visible facet as 0 instead of roundoff\n\
			   - use 'En' to specify a maximum roundoff error less than %2.2g.\n\
			   - options 'Qf', 'Qbb', and 'QR0' may also help\n",
			   qh->DISTround);
		qh_fprintf(qh, fp, 9374, "\
			   \n\
			   To guarantee simplicial output:\n\
			   - use option 'Qt' to triangulate the output\n\
			   - use option 'QJ' to joggle the input points and remove precision errors\n\
			   - use option 'Ft' to triangulate the output by adding points\n\
			   - use exact arithmetic (see \"Imprecision in Qhull\", qh-impre.htm)\n\
			   ");
	}
} /* printhelp_degenerate */

/*-<a                             href="qh-globa_r.htm#TOC"
 >-------------------------------</a><a name="printhelp_narrowhull">-</a>

 qh_printhelp_narrowhull(qh, minangle )
 Warn about a narrow hull

 notes:
 Alternatively, reduce qh_WARNnarrow in user.h

 */
void qh_printhelp_narrowhull(qhT* qh, FILE* fp, realT minangle)
{

	qh_fprintf(qh, fp, 9375, "qhull precision warning: \n\
		   The initial hull is narrow (cosine of min. angle is %.16f).\n\
		   Is the input lower dimensional (e.g., on a plane in 3-d)?  Qhull may\n\
		   produce a wide facet.  Options 'QbB' (scale to unit box) or 'Qbb' (scale\n\
		   last coordinate) may remove this warning.  Use 'Pp' to skip this warning.\n\
		   See 'Limitations' in qh-impre.htm.\n",
		   -minangle); /* convert from angle between normals to angle between facets */
} /* printhelp_narrowhull */

/*-<a                             href="qh-io_r.htm#TOC"
 >-------------------------------</a><a name="printhelp_singular">-</a>

 qh_printhelp_singular(qh, fp )
 prints descriptive message for singular input
 */
void qh_printhelp_singular(qhT* qh, FILE* fp)
{
	facetT*  facet;
	vertexT *vertex, **vertexp;
	realT    min, max, *coord, dist;
	int      i, k;

	qh_fprintf(qh, fp, 9376, "\n\
		   The input to qhull appears to be less than %d dimensional, or a\n\
		   computation has overflowed.\n\n\
		   Qhull could not construct a clearly convex simplex from points:\n",
		   qh->hull_dim);
	qh_printvertexlist(qh, fp, "", qh->facet_list, NULL, qh_ALL);
	if (!qh_QUICKhelp)
		qh_fprintf(qh, fp, 9377, "\n\
			   The center point is coplanar with a facet, or a vertex is coplanar\n\
			   with a neighboring facet.  The maximum round off error for\n\
			   computing distances is %2.2g.  The center point, facets and distances\n\
			   to the center point are as follows:\n\n", qh->DISTround);
	qh_printpointid(qh, fp, "center point", qh->hull_dim, qh->interior_point, qh_IDunknown);
	qh_fprintf(qh, fp, 9378, "\n");
	FORALLfacets
	{
		qh_fprintf(qh, fp, 9379, "facet");
		FOREACHvertex_(facet->vertices)
		    qh_fprintf(qh, fp, 9380, " p%d", qh_pointid(qh, vertex->point));
		zinc_(Zdistio);
		qh_distplane(qh, qh->interior_point, facet, &dist);
		qh_fprintf(qh, fp, 9381, " distance= %4.2g\n", dist);
	}
	if (!qh_QUICKhelp)
	{
		if (qh->HALFspace)
			qh_fprintf(qh, fp, 9382, "\n\
				   These points are the dual of the given halfspaces.  They indicate that\n\
				   the intersection is degenerate.\n");
		qh_fprintf(qh, fp, 9383, "\n\
			   These points either have a maximum or minimum x-coordinate, or\n\
			   they maximize the determinant for k coordinates.  Trial points\n\
			   are first selected from points that maximize a coordinate.\n");
		if (qh->hull_dim >= qh_INITIALmax)
			qh_fprintf(qh, fp, 9384, "\n\
				   Because of the high dimension, the min x-coordinate and max-coordinate\n\
				   points are used if the determinant is non-zero.  Option 'Qs' will\n\
				   do a better, though much slower, job.  Instead of 'Qs', you can change\n\
				   the points by randomly rotating the input with 'QR0'.\n");
	}
	qh_fprintf(qh, fp, 9385, "\nThe min and max coordinates for each dimension are:\n");
	for (k = 0; k < qh->hull_dim; k++)
	{
		min = REALmax;
		max = -REALmin;
		for (i = qh->num_points, coord = qh->first_point + k; i--; coord += qh->hull_dim)
		{
			maximize_(max, *coord);
			minimize_(min, *coord);
		}
		qh_fprintf(qh, fp, 9386, "  %d:  %8.4g  %8.4g  difference= %4.4g\n", k, min, max, max - min);
	}
	if (!qh_QUICKhelp)
	{
		qh_fprintf(qh, fp, 9387, "\n\
			   If the input should be full dimensional, you have several options that\n\
			   may determine an initial simplex:\n\
			   - use 'QJ'  to joggle the input and make it full dimensional\n\
			   - use 'QbB' to scale the points to the unit cube\n\
			   - use 'QR0' to randomly rotate the input for different maximum points\n\
			   - use 'Qs'  to search all points for the initial simplex\n\
			   - use 'En'  to specify a maximum roundoff error less than %2.2g.\n\
			   - trace execution with 'T3' to see the determinant for each point.\n",
			   qh->DISTround);
#if REALfloat
		qh_fprintf(qh, fp, 9388, "\
			   - recompile qhull for realT precision(#define REALfloat 0 in libqhull_r.h).\n");
#endif
		qh_fprintf(qh, fp, 9389, "\n\
			   If the input is lower dimensional:\n\
			   - use 'QJ' to joggle the input and make it full dimensional\n\
			   - use 'Qbk:0Bk:0' to delete coordinate k from the input.  You should\n\
			   pick the coordinate with the least range.  The hull will have the\n\
			   correct topology.\n\
			   - determine the flat containing the points, rotate the points\n\
			   into a coordinate plane, and delete the other coordinates.\n\
			   - add one or more points to make the input full dimensional.\n\
			   ");
	}
} /* printhelp_singular */

/*-<a                             href="qh-globa_r.htm#TOC"
 >-------------------------------</a><a name="user_memsizes">-</a>

 qh_user_memsizes(qh)
 allocate up to 10 additional, quick allocation sizes

 notes:
 increase maximum number of allocations in qh_initqhull_mem()
 */
void qh_user_memsizes(qhT* qh)
{

	QHULL_UNUSED(qh)
	/* qh_memsize(qh, size); */
} /* user_memsizes */

#define qh_QHimport
//#include "libqhull/qhull_ra.h"
#include <libqhull_r/qhull_ra.h>

/*-------------------------------------------------
 -print_summary()

 static void print_summary(qhT* my_qh) {
 facetT *facet;
 int k;

 printf("\n%d vertices and %d facets with normals:\n",
 qh num_vertices, qh num_facets);
 FORALLfacets {
 for (k=0; k < qh hull_dim; k++)
 printf("%6.2g ", facet->normal[k]);
 printf("\n");
 }
 }
 */

#include "v_primitives.h"

void print_summary(qhT* qh)
{
	facetT* facet;
	int     k;

	printf("\n%d vertices and %d facets with normals:\n",
	       qh->num_vertices, qh->num_facets);
	FORALLfacets
	{
		for (k = 0; k < qh->hull_dim; k++)
			printf("%6.2g ", facet->normal[k]);
		printf("\n");
	}
}

#define TV .25
//#define QH_TEST_NUM 32
#define QH_DIMENSIONS 3
static RRandom my_rng;

#include <r4/r4.h>

void vbl_qhull_test(int num_points)
{

	my_rng = r_rand_create_seed(0);

	//int num  = 10;
	/*
	 for ( int i = 0;i < num; i++ )
	 {
	 double  x = r_rand_double(NULL);
	 double y = r_rand_double(NULL);
	 double z  = r_rand_double(NULL);
	 }
	 */
	qhT  my_qh;
	qhT* qh = &my_qh;
	qh_zero(qh, NULL);
	/*
	 double pdata[8*3] = {
	 0.0, 0.0, 0.0,
	 TV, 0.0, 0.0,
	 TV, TV, 0.0,
	 0.0, TV, 0.0,
	 0.0, 0.0, TV,
	 TV, 0.0, TV,
	 TV, TV, TV,
	 0.0, TV, TV,

	 };
	 */

	double* points = calloc(num_points * 3, sizeof(double));
	;

	//if ( !points ){

	//points =
	int tn = (num_points)*3;
	for (int i = 0; i < tn; i += 3)
	{
		RPoint3 p     = v_primitives_random_point_in_sphere(.5);
		points[i + 0] = p.x;
		points[i + 1] = fabs(p.y);
		points[i + 2] = p.z;
		//points[i + 0] = -.5 + r_rand_double(&my_rng); // pdata[i];
		//points[i + 1] = -.5 + r_rand_double(&my_rng); //pdata[i+1];
		//points[i + 2] = -.5 + r_rand_double(&my_rng);
	}
	//}

	const char* q_out = "/tmp/qhull_output.txt";
	const char* q_err = "/tmp/qhull_error.txt";

	FILE* o = fopen(q_out, "w");
	FILE* e = fopen(q_err, "w");

	qh_new_qhull(qh, 3, num_points, points, true, "qhull ", o, e);

	//qh_setvoronoi_all(qh);

	/*	FORALLvertices {
	 for (k=0; k < qh hull_dim; k++)
	 printf("%6.2g ", facet->normal[k]);
	 printf("\n");

	 }
	 */

	//drw_scale_u(.25);

	//ALSO NOPE
	facetT *facet, *neighbor;
	ridgeT *ridge, **ridgep;
	facetT* visible = NULL;

	qh->visit_id = 0;
	//qh visit_id++;
	FORALLfacets
	{
		//printf("facet f%d\n", facet->id);
		qh_makeridges(qh, facet);
		facet->visitid = qh->visit_id;

		int     nv   = facet->vertices->maxsize;
		double* dval = calloc(3 * nv, sizeof(double));

		RLine3* line = r_line3_create();
		for (int i = 0, j = 0; i < nv; i++, j += 2)
		{
			vertexT* v  = (vertexT*)facet->vertices->e[i].p;
			double*  p  = v->point;
			dval[j]     = p[0];
			dval[j + 1] = p[1];
			dval[j + 2] = p[2];
			r_line3_add_point3f(line, p[0], p[1], p[2]);
		}

		drw_fill_set(true);
		drw_alpha(1);
		drw_color_c16(r_app_color_get_bg());
		drw_color(0.0, 0.4, 0.5, 1.0);
		drw_line3_r(line);
		r_line3_destroy(line);
		drw_color_c16(r_app_color_get_fh());

		//drw_color_c16()
		//drw_rawpoly_3d((double*)facet->vertices->e, facet->vertices->maxsize, true);
		//drw_rawpoly_3d(dval, nv, true);

		drw_alpha(1);
		//facet->visitId= qh->visit_id;
		FOREACHridge_(facet->ridges)
		{
			neighbor = otherfacet_(ridge, visible);
			if (neighbor->visitid != qh->visit_id)
			{
				//printf("   ridge r%d between f%d and f%d\n", ridge->id, ridge->top->id, ridge->bottom->id);

				vertexT* a = (vertexT*)ridge->vertices->e[0].p;
				vertexT* b = (vertexT*)ridge->vertices->e[1].p;

				double* pa = a->point;
				double* pb = b->point;
				//drw_fill_set(true);
				//drw_rawpoly_3d(
				//drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);

				//	vertexT* a = (vertexT*)facet->vertices->e[i-1].p;
				//	vertexT* b = (vertexT*)facet->vertices->e[i].p;
			}
		}
	}

	//facetT* facet;

	FORALLfacets
	{
		for (int i = 0; i < facet->vertices->maxsize - 1; i++)
		{
			vertexT* a = facet->vertices->e[i + 0].p;
			vertexT* b = facet->vertices->e[i + 1].p;
			if (!a->point)
				continue;
			if (!b->point)
				continue;

			double* pa = a->point;
			double* pb = b->point;
			drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);
		}
	}

	/*

	 	THIS WORKS LIKE FUCKING FUCK
	for ( int i = 0; i < qh->num_vertices -1; i++ )
	{
		//for ( int j = 0; j < qh->num_vertices; j++ )
		//{

			vertexT va = qh->vertex_list[i];
			vertexT vb = qh->vertex_list[i+1];
			int ia = qh_pointid(qh, va.point);
			int ib = qh_pointid(qh, vb.point);

			printf("%d x %d...\n", ia, ib);
			if ( ia == ib )
				continue;
			//if ( ib < 0 || ia < 0)
			//	continue;

			if (!va.point)
				continue;
			if (! vb.point)
				continue;
			double* pa = va.point;
			double* pb = vb.point;
			qh_printvertex(qh, o, &va);
			qh_printvertex(qh, o, &vb);
			if ( pa && pb )
			{
				drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);

				//double *ba = malloc(sizeof(double)*3);
				//memcpy(ba, pa, 3*sizeof(double));
				//memcpy(ba+1, pa, sizeof(double) * 3);
				//drw_line_3f(ba[0], ba[1], ba[2], pb[0], pb[1], pb[2]);
				//free(ba);

			}//double ay = qh->vertex_list[i+1];
		//}
	}
	*/

	//facetT *facet;
	//int k;

	//vertexT* vertex;
	//printf("\n%d vertices and %d facets with normals:\n",
	//       qh->num_vertices, qh->num_facets);
	//pointT* prev = NULL;

	/*
	 FORALLvertices
	 {
	 printf("%f %f %f\n", vertex->point[0], vertex->point[1], vertex->point[2]);
	 for ( int i = 0; i < vertex->neighbors->maxsize; i++ )
	 {
	 vertexT* n = (vertexT*)vertex->neighbors[i].e;

	 if ( n )
	 {
	 double* p = n->point;
	 if ( p )
	 drw_line_3f(vertex->point[0], vertex->point[1], vertex->point[2], p[0], p[1], p[2]);
	 }
	 }
	 }*/
	/*
	FORALLfacets {
		for (k=0; k < qh->hull_dim; k++)
		{
			qh_printfacet(qh, o, facet);
			pointT *p = qh_getcenter(qh, facet->vertices);

			for ( int i = 1 ;i < facet->vertices->maxsize-1; i++ )
			{
				vertexT* a = (vertexT*)facet->vertices->e[i-1].p;
				vertexT* b = (vertexT*)facet->vertices->e[i].p;
				if ( !a || !b)
					continue;
				double* da = a->point;
				double* db = b->point;

				drw_line_3f(da[0], da[1], da[2], db[0], db[1], db[2]);
			}

			/*FORALLvertex_(facet->vertices)
			 {

			 }
			 */
	/*
			 if ( prev )
			 {
			 drw_line_3f(prev[0], prev[1], prev[2], p[0], p[1], p[2] );
			 }

			 //FORALL
			 prev = p;
			 //setT* verts = facet->vertices;

			 //FORALLvertex_(facet->vertices)
			 //{
			 //	printf("%f %f %f\n", vertex->point[0], vertex->point[1], vertex->point[2]);
			 //}
			 /*
			 for ( int j = 1; j < verts->maxsize-1; j++ )
			 {

			 setelemT se = verts->e[j];

			 vertexT* v = se.p;
			 }


			drw_point3f(p[0], p[1], p[2]);
			//printf("%6.2g ", facet->normal[k]);
		}
		//printf("\n");
	}
	*/

	//print_summary(qh);
	/*
	 for ( int i = 0; i < qh->num_facets; i++ )
	 {
	 facetT f = qh->facet_list[i];
	 setT *verts = f.vertices;
	 if ( !verts )
	 continue;
	 for ( int j = 0; j < verts->maxsize; j ++ )
	 {
	 //setelemT e = verts->e[j];
	 //printf("%f\n", *(double*)e.p);

	 }
	 //for ( int j = 0 ; j < verts. )

	 //double* c = f.center;
	 //if ( f.good )
	 //	drw_point3f(c[0], c[1], c[2] );
	 }*/

	//qh->PRINTout();
	//print_summary(qh);
	/*

	 int qh_new_qhull(qhT *qh, int dim, int numpoints, coordT *points, boolT ismalloc,
	 char *qhull_cmd, FILE *outfile, FILE *errfile) {

	 */

	qh_freeqhull(qh, true);

	fclose(o);
	fclose(e);
}

void vbl_qhull_draw(RQHull* data)
{

	qhT* qh = data->src;

	//ALSO NOPE
	facetT *facet, *neighbor;
	ridgeT *ridge, **ridgep;
	facetT* visible = NULL;

	qh->visit_id = 0;
	//qh visit_id++;
	FORALLfacets
	{
		//printf("facet f%d\n", facet->id);
		qh_makeridges(qh, facet);
		facet->visitid = qh->visit_id;

		int     nv   = facet->vertices->maxsize;
		double* dval = calloc(3 * nv, sizeof(double));

		RLine3* line = r_line3_create();
		for (int i = 0, j = 0; i < nv; i++, j += 2)
		{
			vertexT* v  = (vertexT*)facet->vertices->e[i].p;
			double*  p  = v->point;
			dval[j]     = p[0];
			dval[j + 1] = p[1];
			dval[j + 2] = p[2];
			r_line3_add_point3f(line, p[0], p[1], p[2]);
		}
		drw_blend_set(DRW_BLEND_MODE_NORMAL);
		glEnable(GL_DEPTH_TEST);
		//glLineWidth(4);
		drw_fill_set(true);
		drw_alpha(.9);
		drw_color_c16(r_app_color_get_bg());
		//drw_color(0.0, 0.4, 0.5, 1.0);
		drw_line3_r(line);
		r_line3_destroy(line);
		drw_color_c16(r_app_color_get_fh());

		drw_alpha(1);
		//facet->visitId= qh->visit_id;
		FOREACHridge_(facet->ridges)
		{
			neighbor = otherfacet_(ridge, visible);
			if (neighbor->visitid != qh->visit_id)
			{

				vertexT* a = (vertexT*)ridge->vertices->e[0].p;
				vertexT* b = (vertexT*)ridge->vertices->e[1].p;

				double* pa = a->point;
				double* pb = b->point;
			}
		}
	}

	//facetT* facet;

	FORALLfacets
	{
		for (int i = 0; i < facet->vertices->maxsize - 1; i++)
		{
			vertexT* a = facet->vertices->e[i + 0].p;
			vertexT* b = facet->vertices->e[i + 1].p;
			if (!a->point)
				continue;
			if (!b->point)
				continue;

			double* pa = a->point;
			double* pb = b->point;
			drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);
		}
	}
}

void vbl_qhull_draw_fill(RQHull* data)
{
	qhT* qh = data->src;

	//ALSO NOPE
	facetT *facet, *neighbor;
	ridgeT *ridge, **ridgep;
	facetT* visible = NULL;

	qh->visit_id = 0;
	//qh visit_id++;
	FORALLfacets
	{
		//printf("facet f%d\n", facet->id);
		qh_makeridges(qh, facet);
		facet->visitid = qh->visit_id;

		int     nv   = facet->vertices->maxsize;
		double* dval = calloc(3 * nv, sizeof(double));

		RLine3* line = r_line3_create();
		for (int i = 0, j = 0; i < nv; i++, j += 2)
		{
			vertexT* v  = (vertexT*)facet->vertices->e[i].p;
			double*  p  = v->point;
			dval[j]     = p[0];
			dval[j + 1] = p[1];
			dval[j + 2] = p[2];
			r_line3_add_point3f(line, p[0], p[1], p[2]);
		}
		drw_blend_set(DRW_BLEND_MODE_NORMAL);
		glEnable(GL_DEPTH_TEST);
		//glLineWidth(4);
		drw_fill_set(true);
		//drw_alpha(.9);
		drw_color_c16(r_app_color_get_bg());
		//drw_color(0.0, 0.4, 0.5, 1.0);
		drw_line3_r(line);
		r_line3_destroy(line);
		drw_color_c16(r_app_color_get_fh());
	}
}

void vbl_qhull_draw_edges(RQHull* data)
{
	qhT* qh = data->src;

	//drw_scale_u(.25);

	//ALSO NOPE
	facetT *facet, *neighbor;
	ridgeT *ridge, **ridgep;
	facetT* visible = NULL;

	qh->visit_id = 0;
	//qh visit_id++;
	FORALLfacets
	{
		//printf("facet f%d\n", facet->id);
		qh_makeridges(qh, facet);
		facet->visitid = qh->visit_id;

		//facet->visitId= qh->visit_id;
		FOREACHridge_(facet->ridges)
		{
			neighbor = otherfacet_(ridge, visible);
			if (neighbor->visitid != qh->visit_id)
			{

				vertexT* a = (vertexT*)ridge->vertices->e[0].p;
				vertexT* b = (vertexT*)ridge->vertices->e[1].p;

				double* pa = a->point;
				double* pb = b->point;
				drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);
			}
		}
	}

	//facetT* facet;

	FORALLfacets
	{
		for (int i = 0; i < facet->vertices->maxsize - 1; i++)
		{
			vertexT* a = facet->vertices->e[i + 0].p;
			vertexT* b = facet->vertices->e[i + 1].p;
			if (!a->point)
				continue;
			if (!b->point)
				continue;

			double* pa = a->point;
			double* pb = b->point;
			drw_line_3f(pa[0], pa[1], pa[2], pb[0], pb[1], pb[2]);
		}
	}
}

RQHull* vbl_qhull_create(int num_points)
{

	qhT* qh = calloc(1, sizeof(qhT)); //my_qh;
	qh_zero(qh, NULL);

	double* points = calloc(num_points * 3, sizeof(double));

	int tn = (num_points)*3;
	for (int i = 0; i < tn; i += 3)
	{
		RPoint3 p     = v_primitives_random_point_in_sphere(.5);
		points[i + 0] = p.x;
		points[i + 1] = (p.y);
		points[i + 2] = p.z;
	}

	qh_new_qhull(qh, 3, num_points, points, true, "qhull ", NULL, NULL);

	//qh_setvoronoi_all(qh);

	RQHull* rec = calloc(1, sizeof(RQHull));
	rec->src    = qh;
	return rec;
}

void vbl_qhull_destroy(RQHull* data)
{
	qh_freeqhull(data->src, true);
}

RQHull* vbl_qhull_create2d(RLine* line)
{
	qhT* qh = calloc(1, sizeof(qhT)); //my_qh;
	qh_zero(qh, NULL);

	double* points = calloc(line->num * 2, sizeof(double));

	int tn = (line->num) * 2;
	for (int i = 0; i < tn; i += 2)
	{
		RPoint p      = line->data[i]; //v_primitives_random_point_in_sphere(.5);
		points[i + 0] = p.x;
		points[i + 1] = p.y;
		//points[i + 2] = p.z;
	}

	qh_new_qhull(qh, 2, line->num, points, true, "qhull ", NULL, NULL);

	//qh_setvoronoi_all(qh);

	RQHull* rec = calloc(1, sizeof(RQHull));
	rec->src    = qh;
	return rec;
}

void vbl_qhull_destroy2d(RQHull* hull)
{
}


#endif

