extern crate glib;
extern crate cairo;
extern crate cairo_sys;
extern crate libc;

use self::glib::translate::*;

use state::RsvgState;
use path_builder::RsvgPathBuilder;

pub enum RsvgDrawingCtx {}

pub enum RsvgNode {}

extern "C" {
    fn rsvg_drawing_ctx_get_dpi (draw_ctx: *const RsvgDrawingCtx,
                                 out_dpi_x: *mut f64,
                                 out_dpi_y: *mut f64);

    fn rsvg_drawing_ctx_get_normalized_font_size (draw_ctx: *const RsvgDrawingCtx) -> f64;

    fn rsvg_drawing_ctx_get_view_box_size (draw_ctx: *const RsvgDrawingCtx,
                                           out_x: *mut f64,
                                           out_y: *mut f64);

    fn rsvg_drawing_ctx_push_view_box (draw_ctx: *const RsvgDrawingCtx,
                                       width:     f64,
                                       height:    f64);

    fn rsvg_drawing_ctx_pop_view_box (draw_ctx: *const RsvgDrawingCtx);

    fn rsvg_drawing_ctx_acquire_node (draw_ctx: *const RsvgDrawingCtx,
                                      url:      *const libc::c_char) -> *mut RsvgNode;

    fn rsvg_drawing_ctx_release_node (draw_ctx: *const RsvgDrawingCtx,
                                      node:     *mut RsvgNode);

    fn rsvg_state_reinherit_top (draw_ctx: *const RsvgDrawingCtx,
                                 state: *mut RsvgState,
                                 dominate: libc::c_int);

    fn rsvg_render_path_builder (draw_ctx: *const RsvgDrawingCtx,
                                 builder: *const RsvgPathBuilder);

    fn rsvg_cairo_get_cairo_context (draw_ctx: *const RsvgDrawingCtx) -> *mut cairo_sys::cairo_t;
}

pub fn get_dpi (draw_ctx: *const RsvgDrawingCtx) -> (f64, f64) {
    let mut dpi_x: f64 = 0.0;
    let mut dpi_y: f64 = 0.0;

    unsafe { rsvg_drawing_ctx_get_dpi (draw_ctx, &mut dpi_x, &mut dpi_y); }

    (dpi_x, dpi_y)
}


pub fn get_normalized_font_size (draw_ctx: *const RsvgDrawingCtx) -> f64 {
    unsafe { rsvg_drawing_ctx_get_normalized_font_size (draw_ctx) }
}

pub fn get_view_box_size (draw_ctx: *const RsvgDrawingCtx) -> (f64, f64) {
    let mut w: f64 = 0.0;
    let mut h: f64 = 0.0;

    unsafe { rsvg_drawing_ctx_get_view_box_size (draw_ctx, &mut w, &mut h); }

    (w, h)
}

pub fn push_view_box (draw_ctx: *const RsvgDrawingCtx,
                      width:     f64,
                      height:    f64)
{
    unsafe { rsvg_drawing_ctx_push_view_box (draw_ctx, width, height); }
}

pub fn pop_view_box (draw_ctx: *const RsvgDrawingCtx) {
    unsafe { rsvg_drawing_ctx_pop_view_box (draw_ctx); }
}

pub fn acquire_node (draw_ctx: *const RsvgDrawingCtx,
                     url:      &str) -> *mut RsvgNode {
    unsafe { rsvg_drawing_ctx_acquire_node (draw_ctx, str::to_glib_none (url).0) }
}

pub fn release_node (draw_ctx: *const RsvgDrawingCtx,
                     node:     *mut RsvgNode) {
    unsafe { rsvg_drawing_ctx_release_node (draw_ctx, node); }
}

pub fn state_reinherit_top (draw_ctx: *const RsvgDrawingCtx,
                            state: *mut RsvgState,
                            dominate: i32) {
    unsafe { rsvg_state_reinherit_top (draw_ctx, state, dominate); }
}

pub fn render_path_builder (draw_ctx: *const RsvgDrawingCtx,
                            builder: &RsvgPathBuilder)
{
    unsafe { rsvg_render_path_builder (draw_ctx, builder); }
}

pub fn get_cairo_context (draw_ctx: *const RsvgDrawingCtx) -> cairo::Context
{
    unsafe {
        let raw_cr = rsvg_cairo_get_cairo_context (draw_ctx);

        let cr = cairo::Context::from_glib_none (raw_cr);

        cr
    }
}
