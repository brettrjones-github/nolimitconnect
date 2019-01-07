use std::cell::RefCell;

use drawing_ctx;
use drawing_ctx::*;
use handle::RsvgHandle;
use node::*;
use path_builder::*;
use path_parser;
use property_bag;
use property_bag::*;
use state::RsvgState;

struct NodePath {
    state: *mut RsvgState,
    builder: RefCell<RsvgPathBuilder>
}

impl NodePath {
    fn new (state: *mut RsvgState) -> NodePath {
        NodePath {
            state: state,
            builder: RefCell::new (RsvgPathBuilder::new ())
        }
    }
}

impl NodeTrait for NodePath {
    fn set_atts (&self, _: *const RsvgHandle, pbag: *const RsvgPropertyBag) {
        if let Some (value) = property_bag::lookup (pbag, "d") {
            let mut builder = self.builder.borrow_mut ();

            if let Err (_) = path_parser::parse_path_into_builder (&value, &mut *builder) {
                // FIXME: we don't propagate errors upstream, but creating a partial
                // path is OK per the spec
            }
        }
    }

    fn draw (&self, draw_ctx: *const RsvgDrawingCtx, dominate: i32) {
        drawing_ctx::state_reinherit_top (draw_ctx, self.state, dominate);
        drawing_ctx::render_path_builder (draw_ctx, & *self.builder.borrow ());
    }
}
