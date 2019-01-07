use std::f64;

extern crate cairo;
extern crate cairo_sys;

#[repr(C)]
pub struct RsvgPathBuilder {
    path_segments: Vec<cairo::PathSegment>,
}

impl RsvgPathBuilder {
    pub fn new () -> RsvgPathBuilder {
        let builder = RsvgPathBuilder {
            path_segments: Vec::new ()
        };

        builder
    }

    pub fn move_to (&mut self, x: f64, y: f64) {
        self.path_segments.push (cairo::PathSegment::MoveTo ((x, y)));
    }

    pub fn line_to (&mut self, x: f64, y: f64) {
        self.path_segments.push (cairo::PathSegment::LineTo ((x, y)));
    }

    pub fn curve_to (&mut self, x2: f64, y2: f64, x3: f64, y3: f64, x4: f64, y4: f64) {
        self.path_segments.push (cairo::PathSegment::CurveTo ((x2, y2), (x3, y3), (x4, y4)));
    }

    pub fn close_path (&mut self) {
        self.path_segments.push (cairo::PathSegment::ClosePath);
    }

    pub fn get_path_segments (&self) -> &Vec<cairo::PathSegment> {
        &self.path_segments
    }

    /**
     * x1/y1: starting coordinates
     * rx/ry: radiuses before rotation
     * x_axis_rotation: Rotation angle for axes, in degrees
     * is_large_arc: false for arc length <= 180, true for arc >= 180
     * is_sweep: false for negative angle, true for positive angle
     * x2/y2: ending coordinates
     */
    pub fn arc (&mut self,
                x1: f64, y1: f64,
                mut rx: f64, mut ry: f64,
                x_axis_rotation: f64,
                is_large_arc: bool,
                is_sweep: bool,
                x2: f64, y2: f64) {
        /* See Appendix F.6 Elliptical arc implementation notes
        http://www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes */

        let f: f64;
        let sinf: f64;
        let cosf: f64;
        let x1_: f64;
        let y1_: f64;
        let cx_: f64;
        let cy_: f64;
        let cx: f64;
        let cy: f64;
        let gamma: f64;
        let mut theta1: f64;
        let mut delta_theta: f64;
        let mut k1: f64;
        let mut k2: f64;
        let k3: f64;
        let k4: f64;
        let mut k5: f64;
        let n_segs: i32;

        if x1 == x2 && y1 == y2 {
            return;
        }

        /* X-axis */
        f = x_axis_rotation * f64::consts::PI / 180.0;
        sinf = f.sin ();
        cosf = f.cos ();

        rx = rx.abs ();
        ry = ry.abs ();

        /* Check the radius against floading point underflow.
        See http://bugs.debian.org/508443 */
        if (rx < f64::EPSILON) || (ry < f64::EPSILON) {
            self.line_to (x2, y2);
            return;
        }

        k1 = (x1 - x2) / 2.0;
        k2 = (y1 - y2) / 2.0;

        x1_ = cosf * k1 + sinf * k2;
        y1_ = -sinf * k1 + cosf * k2;

        gamma = (x1_ * x1_) / (rx * rx) + (y1_ * y1_) / (ry * ry);
        if gamma > 1.0 {
            rx *= gamma.sqrt ();
            ry *= gamma.sqrt ();
        }

        /* Compute the center */

        k1 = rx * rx * y1_ * y1_ + ry * ry * x1_ * x1_;
        if k1 == 0.0 {
            return;
        }

        k1 = ((rx * rx * ry * ry) / k1 - 1.0).abs ().sqrt ();
        if is_sweep == is_large_arc {
            k1 = -k1;
        }

        cx_ = k1 * rx * y1_ / ry;
        cy_ = -k1 * ry * x1_ / rx;

        cx = cosf * cx_ - sinf * cy_ + (x1 + x2) / 2.0;
        cy = sinf * cx_ + cosf * cy_ + (y1 + y2) / 2.0;

        /* Compute start angle */

        k1 = (x1_ - cx_) / rx;
        k2 = (y1_ - cy_) / ry;
        k3 = (-x1_ - cx_) / rx;
        k4 = (-y1_ - cy_) / ry;

        k5 = (k1 * k1 + k2 * k2).abs ().sqrt ();
        if k5 == 0.0 {
            return;
        }

        k5 = k1 / k5;
        k5 = clamp (k5, -1.0, 1.0);
        theta1 = k5.acos ();
        if k2 < 0.0 {
            theta1 = -theta1;
        }

        /* Compute delta_theta */

        k5 = ((k1 * k1 + k2 * k2) * (k3 * k3 + k4 * k4)).abs ().sqrt ();
        if k5 == 0.0 {
            return;
        }

        k5 = (k1 * k3 + k2 * k4) / k5;
        k5 = clamp (k5, -1.0, 1.0);
        delta_theta = k5.acos ();
        if k1 * k4 - k3 * k2 < 0.0 {
            delta_theta = -delta_theta;
        }

        if is_sweep && delta_theta < 0.0 {
            delta_theta += f64::consts::PI * 2.0;
        } else if !is_sweep && delta_theta > 0.0 {
            delta_theta -= f64::consts::PI * 2.0;
        }

        /* Now draw the arc */

        n_segs = (delta_theta / (f64::consts::PI * 0.5 + 0.001)).abs ().ceil () as i32;
        let n_segs_dbl = n_segs as f64;

        for i in 0 .. n_segs {
            self.arc_segment (cx, cy,
                              theta1 + i as f64 * delta_theta / n_segs_dbl,
                              theta1 + (i + 1) as f64 * delta_theta / n_segs_dbl,
                              rx, ry,
                              x_axis_rotation);
        }
    }

    fn arc_segment (&mut self,
                    xc: f64, yc: f64,
                    th0: f64, th1: f64,
                    rx: f64, ry: f64,
                    x_axis_rotation: f64) {
        let x1: f64;
        let y1: f64;
        let x2: f64;
        let y2: f64;
        let x3: f64;
        let y3: f64;
        let t: f64;
        let th_half: f64;
        let f: f64;
        let sinf: f64;
        let cosf: f64;

        f = x_axis_rotation * f64::consts::PI / 180.0;
        sinf = f.sin ();
        cosf = f.cos ();

        th_half = 0.5 * (th1 - th0);
        t = (8.0 / 3.0) * (th_half * 0.5).sin () * (th_half * 0.5).sin () / th_half.sin ();
        x1 = rx * (th0.cos () - t * th0.sin ());
        y1 = ry * (th0.sin () + t * th0.cos ());
        x3 = rx * th1.cos ();
        y3 = ry * th1.sin ();
        x2 = x3 + rx * (t * th1.sin ());
        y2 = y3 + ry * (-t * th1.cos ());

        self.curve_to (xc + cosf * x1 - sinf * y1,
                       yc + sinf * x1 + cosf * y1,
                       xc + cosf * x2 - sinf * y2,
                       yc + sinf * x2 + cosf * y2,
                       xc + cosf * x3 - sinf * y3,
                       yc + sinf * x3 + cosf * y3);
    }
}

fn clamp (val: f64, low: f64, high: f64) -> f64 {
    if val < low {
        low
    } else if val > high {
        high
    } else {
        val
    }
}

#[no_mangle]
pub unsafe extern fn rsvg_path_builder_new () -> *mut RsvgPathBuilder {
    let builder = RsvgPathBuilder::new ();

    let boxed_builder = Box::new (builder);

    Box::into_raw (boxed_builder)
}

#[no_mangle]
pub unsafe extern fn rsvg_path_builder_destroy (raw_builder: *mut RsvgPathBuilder) {
    assert! (!raw_builder.is_null ());

    let _ = Box::from_raw (raw_builder);
}

#[no_mangle]
pub extern fn rsvg_path_builder_move_to (raw_builder: *mut RsvgPathBuilder,
                                         x: f64,
                                         y: f64) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    builder.move_to (x, y);
}

#[no_mangle]
pub extern fn rsvg_path_builder_line_to (raw_builder: *mut RsvgPathBuilder,
                                         x: f64,
                                         y: f64) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    builder.line_to (x, y);
}

#[no_mangle]
pub extern fn rsvg_path_builder_curve_to (raw_builder: *mut RsvgPathBuilder,
                                          x2: f64, y2: f64,
                                          x3: f64, y3: f64,
                                          x4: f64, y4: f64) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    builder.curve_to (x2, y2, x3, y3, x4, y4);
}

#[no_mangle]
pub extern fn rsvg_path_builder_arc (raw_builder: *mut RsvgPathBuilder,
                                     x1: f64, y1: f64,
                                     rx: f64, ry: f64,
                                     x_axis_rotation: f64,
                                     large_arc_flag: bool,
                                     sweep_flag: bool,
                                     x2: f64, y2: f64) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    builder.arc (x1, y1,
                 rx, ry,
                 x_axis_rotation,
                 large_arc_flag,
                 sweep_flag,
                 x2, y2);
}


#[no_mangle]
pub extern fn rsvg_path_builder_close_path (raw_builder: *mut RsvgPathBuilder) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    builder.close_path ();
}

#[no_mangle]
pub extern fn rsvg_path_builder_add_to_cairo_context (raw_builder: *mut RsvgPathBuilder, cr: *mut cairo_sys::cairo_t) {
    assert! (!raw_builder.is_null ());

    let builder: &mut RsvgPathBuilder = unsafe { &mut (*raw_builder) };

    unsafe {
        let path_segments = builder.get_path_segments ();

        for s in path_segments {
            match *s {
                cairo::PathSegment::MoveTo ((x, y)) => {
                    cairo_sys::cairo_move_to (cr, x, y);
                },

                cairo::PathSegment::LineTo ((x, y)) => {
                    cairo_sys::cairo_line_to (cr, x, y);
                },

                cairo::PathSegment::CurveTo ((x2, y2), (x3, y3), (x4, y4)) => {
                    cairo_sys::cairo_curve_to (cr, x2, y2, x3, y3, x4, y4);
                },

                cairo::PathSegment::ClosePath => {
                    cairo_sys::cairo_close_path (cr);
                }
            }
        }
    }
}
