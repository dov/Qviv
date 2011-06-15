! include( common.pri ) {
    error( Couldn't find the common.pri file! )
}

QMAKE_CXXFLAGS+= -Wno-unused-parameter

HEADERS = QvivImageViewer.h QvivWidget.h QvivData.h QvivRenderer.h QvivPainter.h QvivPainterAgg.h 
SOURCES = test-qviv-widget.cc \
    QvivRenderer.cc \
    QvivImageViewer.cc \
    QvivWidget.cc \
    QvivData.cc \
    QvivPainterAgg.cc \
    giv_agg_arrowhead.cc \
    agg/agg_arc.cpp \
    agg/agg_arrowhead.cpp \
    agg/agg_bezier_arc.cpp \
    agg/agg_bspline.cpp \
    agg/agg_curves.cpp \
    agg/agg_embedded_raster_fonts.cpp \
    agg/agg_gsv_text.cpp \
    agg/agg_image_filters.cpp \
    agg/agg_line_aa_basics.cpp \
    agg/agg_line_profile_aa.cpp \
    agg/agg_rounded_rect.cpp \
    agg/agg_sqrt_tables.cpp \
    agg/agg_trans_affine.cpp \
    agg/agg_trans_double_path.cpp \
    agg/agg_trans_single_path.cpp \
    agg/agg_trans_warp_magnifier.cpp \
    agg/agg_vcgen_bspline.cpp \
    agg/agg_vcgen_contour.cpp \
    agg/agg_vcgen_dash.cpp \
    agg/agg_vcgen_markers_term.cpp \
    agg/agg_vcgen_smooth_poly1.cpp \
    agg/agg_vcgen_stroke.cpp \
    agg/agg_vpgen_clip_polygon.cpp \
    agg/agg_vpgen_clip_polyline.cpp \
    agg/agg_vpgen_segmentator.cpp 

CONFIG += qt debug 

