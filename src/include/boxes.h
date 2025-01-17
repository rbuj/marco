/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Simple box operations */

/*
 * Copyright (C) 2005, 2006 Elijah Newren
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef META_BOXES_H
#define META_BOXES_H

#include <glib.h>

#include "common.h"

typedef enum {
  META_EDGE_WINDOW,
  META_EDGE_XINERAMA,
  META_EDGE_SCREEN
} MetaEdgeType;

typedef struct _MetaRectangle MetaRectangle;
struct _MetaRectangle {
  int x;
  int y;
  int width;
  int height;
};

typedef struct _MetaStrut MetaStrut;
struct _MetaStrut {
  MetaRectangle rect;
  MetaSide side;
  MetaEdgeType edge;
};

#define BOX_LEFT(box) ((box).x)                  /* Leftmost pixel of rect */
#define BOX_RIGHT(box) ((box).x + (box).width)   /* One pixel past right   */
#define BOX_TOP(box) ((box).y)                   /* Topmost pixel of rect  */
#define BOX_BOTTOM(box) ((box).y + (box).height) /* One pixel past bottom  */

typedef enum {
  FIXED_DIRECTION_NONE = 0,
  FIXED_DIRECTION_X = 1 << 0,
  FIXED_DIRECTION_Y = 1 << 1,
} FixedDirections;

typedef struct _MetaEdge MetaEdge;
struct _MetaEdge {
  MetaRectangle rect; /* width or height should be 1 */
  MetaSide side_type;
  MetaEdgeType edge_type;
};

/* Output functions -- note that the output buffer had better be big enough:
 *   rect_to_string:   RECT_LENGTH
 *   region_to_string: (RECT_LENGTH+strlen(separator_string)) *
 *                     g_list_length (region)
 *   edge_to_string:   EDGE_LENGTH
 *   edge_list_to_...: (EDGE_LENGTH+strlen(separator_string)) *
 *                     g_list_length (edge_list)
 */
#define RECT_LENGTH 27
#define EDGE_LENGTH 37
char *meta_rectangle_to_string(const MetaRectangle *rect, char *output);
char *meta_rectangle_region_to_string(GList *region,
                                      const char *separator_string,
                                      char *output);
char *meta_rectangle_edge_to_string(const MetaEdge *edge, char *output);
char *meta_rectangle_edge_list_to_string(GList *edge_list,
                                         const char *separator_string,
                                         char *output);

/* Function to make initializing a rect with a single line of code easy */
MetaRectangle meta_rect(int x, int y, int width, int height);

/* Basic comparison functions */
int meta_rectangle_area(const MetaRectangle *rect);
gboolean meta_rectangle_intersect(const MetaRectangle *src1,
                                  const MetaRectangle *src2,
                                  MetaRectangle *dest);
gboolean meta_rectangle_equal(const MetaRectangle *src1,
                              const MetaRectangle *src2);

/* Find the bounding box of the union of two rectangles */
void meta_rectangle_union(const MetaRectangle *rect1,
                          const MetaRectangle *rect2, MetaRectangle *dest);

/* overlap is similar to intersect but doesn't provide location of
 * intersection information.
 */
gboolean meta_rectangle_overlap(const MetaRectangle *rect1,
                                const MetaRectangle *rect2);

/* vert_overlap means ignore the horizontal location and ask if the
 * vertical parts overlap.  An alternate way to think of it is "Does there
 * exist a way to shift either rect horizontally so that the two rects
 * overlap?"  horiz_overlap is similar.
 */
gboolean meta_rectangle_vert_overlap(const MetaRectangle *rect1,
                                     const MetaRectangle *rect2);
gboolean meta_rectangle_horiz_overlap(const MetaRectangle *rect1,
                                      const MetaRectangle *rect2);

/* could_fit_rect determines whether "outer_rect" is big enough to contain
 * inner_rect.  contains_rect checks whether it actually contains it.
 */
gboolean meta_rectangle_could_fit_rect(const MetaRectangle *outer_rect,
                                       const MetaRectangle *inner_rect);
gboolean meta_rectangle_contains_rect(const MetaRectangle *outer_rect,
                                      const MetaRectangle *inner_rect);

/* Resize old_rect to the given new_width and new_height, but store the
 * result in rect.  NOTE THAT THIS IS RESIZE ONLY SO IT CANNOT BE USED FOR
 * A MOVERESIZE OPERATION (that simplies the routine a little bit as it
 * means there's no difference between NorthWestGravity and StaticGravity.
 * Also, I lied a little bit--technically, you could use it in a MoveResize
 * operation if you muck with old_rect just right).
 */
void meta_rectangle_resize_with_gravity(const MetaRectangle *old_rect,
                                        MetaRectangle *rect, int gravity,
                                        int new_width, int new_height);

/* find a list of rectangles with the property that a window is contained
 * in the given region if and only if it is contained in one of the
 * rectangles in the list.
 *
 * In this case, the region is given by taking basic_rect, removing from
 * it the intersections with all the rectangles in the all_struts list,
 * then expanding all the rectangles in the resulting list by the given
 * amounts on each side.
 *
 * See boxes.c for more details.
 */
GList *meta_rectangle_get_minimal_spanning_set_for_region(
    const MetaRectangle *basic_rect, const GSList *all_struts,
    gboolean skip_middle_struts);

/* Expand all rectangles in region by the given amount on each side */
GList *meta_rectangle_expand_region(GList *region, const int left_expand,
                                    const int right_expand,
                                    const int top_expand,
                                    const int bottom_expand);
/* Same as for meta_rectangle_expand_region except that rectangles not at
 * least min_x or min_y in size are not expanded in that direction
 */
GList *meta_rectangle_expand_region_conditionally(
    GList *region, const int left_expand, const int right_expand,
    const int top_expand, const int bottom_expand, const int min_x,
    const int min_y);

/* Expand rect in direction to the size of expand_to, and then clip out any
 * overlapping struts oriented orthognal to the expansion direction.  (Think
 * horizontal or vertical maximization)
 */
void meta_rectangle_expand_to_avoiding_struts(MetaRectangle *rect,
                                              const MetaRectangle *expand_to,
                                              const MetaDirection direction,
                                              const GSList *all_struts);

/* could_fit_in_region determines whether one of the spanning_rects is
 * big enough to contain rect.  contained_in_region checks whether one
 * actually contains it.
 */
gboolean meta_rectangle_could_fit_in_region(const GList *spanning_rects,
                                            const MetaRectangle *rect);
gboolean meta_rectangle_contained_in_region(const GList *spanning_rects,
                                            const MetaRectangle *rect);
gboolean meta_rectangle_overlaps_with_region(const GList *spanning_rects,
                                             const MetaRectangle *rect);

/* Make the rectangle small enough to fit into one of the spanning_rects,
 * but make it no smaller than min_size.
 */
void meta_rectangle_clamp_to_fit_into_region(const GList *spanning_rects,
                                             FixedDirections fixed_directions,
                                             MetaRectangle *rect,
                                             const MetaRectangle *min_size);

/* Clip the rectangle so that it fits into one of the spanning_rects, assuming
 * it overlaps with at least one of them
 */
void meta_rectangle_clip_to_region(const GList *spanning_rects,
                                   FixedDirections fixed_directions,
                                   MetaRectangle *rect);

/* Shove the rectangle into one of the spanning_rects, assuming it fits in
 * one of them.
 */
void meta_rectangle_shove_into_region(const GList *spanning_rects,
                                      FixedDirections fixed_directions,
                                      MetaRectangle *rect);

/* Finds the point on the line connecting (x1,y1) to (x2,y2) which is closest
 * to (px, py).  Useful for finding an optimal rectangle size when given a
 * range between two sizes that are all candidates.
 */
void meta_rectangle_find_linepoint_closest_to_point(double x1, double y1,
                                                    double x2, double y2,
                                                    double px, double py,
                                                    double *valx, double *valy);

/***************************************************************************/
/*                                                                         */
/* Switching gears to code for edges instead of just rectangles            */
/*                                                                         */
/***************************************************************************/

/* Return whether an edge overlaps or is adjacent to the rectangle in the
 * nonzero-width dimension of the edge.
 */
gboolean meta_rectangle_edge_aligns(const MetaRectangle *rect,
                                    const MetaEdge *edge);

/* Compare two edges, so that sorting functions can put a list of edges in
 * canonical order.
 */
gint meta_rectangle_edge_cmp(gconstpointer a, gconstpointer b);

/* Compare two edges, so that sorting functions can put a list of edges in
 * order.  This function doesn't separate left edges first, then right edges,
 * etc., but rather compares only upon location.
 */
gint meta_rectangle_edge_cmp_ignore_type(gconstpointer a, gconstpointer b);

/* Removes an parts of edges in the given list that intersect any box in the
 * given rectangle list.  Returns the result.
 */
GList *meta_rectangle_remove_intersections_with_boxes_from_edges(
    GList *edges, const GSList *rectangles);

/* Finds all the edges of an onscreen region, returning a GList* of
 * MetaEdgeRect's.
 */
GList *meta_rectangle_find_onscreen_edges(const MetaRectangle *basic_rect,
                                          const GSList *all_struts);

/* Finds edges between adjacent xineramas which are not covered by the given
 * struts.
 */
GList *meta_rectangle_find_nonintersected_xinerama_edges(
    const MetaRectangle *screen_rect, const GList *xinerama_rects,
    const GSList *all_struts);

#endif /* META_BOXES_H */
