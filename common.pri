release:DESTDIR = release
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.obj
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui

QMAKE_CXXFLAGS+= -Wno-unused-parameter

HEADERS = QvivImageViewer.h QvivWidget.h QvivData.h QvivRenderer.h QvivPainter.h QvivPainterAgg.h QvivX11Colors.h QvivHistogram.h
SOURCES =  \
    QvivRenderer.cc \
    QvivImageViewer.cc \
    QvivWidget.cc \
    QvivData.cc \
    QvivPainterAgg.cc \
    QvivX11Colors.cc \
    QvivHistogram.cc \
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
