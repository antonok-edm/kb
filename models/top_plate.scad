/* 
 * Top keyboard plate
 * Author: Anton Lazarev
 * https://github.com/antonok-edm/kb
 *
 * Units are in millimeters.
 */

$fn=400;
HOLE_RECT_X=271.7;
HOLE_RECT_Y=100.25;
OUTER_BEVEL_RADIUS=3;
HOLE_RADIUS=1.5;
EDGE_OUTSET=2.5;

SWITCH_SLOT_WIDTH=14;
SWITCH_SLOT_HEIGHT=SWITCH_SLOT_WIDTH;

SWITCH_INDENT_DEPTH=.8;
SWITCH_INDENT_HEIGHT_OFFSET=1;
SWITCH_INDENT_WIDTH=3.1;

SWITCH_DELTA_X=19.05;
SWITCH_DELTA_Y=SWITCH_DELTA_X;
SWITCH_BORDER_X=5.025;
SWITCH_BORDER_Y=SWITCH_BORDER_X;

//Reference variables
PLATE_X_DIM=HOLE_RECT_X+EDGE_OUTSET*2;
PLATE_Y_DIM=HOLE_RECT_Y+EDGE_OUTSET*2;
UNITS="mm";
echo("Plate width: ", PLATE_X_DIM, UNITS);
echo("Plate height: ", PLATE_Y_DIM, UNITS);

module PlateOutline(tolerance) {
    translate([EDGE_OUTSET+tolerance, EDGE_OUTSET+tolerance]) difference() {
        // Plate Outline
        offset(tolerance) hull() {
            translate([OUTER_BEVEL_RADIUS-EDGE_OUTSET,OUTER_BEVEL_RADIUS-EDGE_OUTSET]) circle(OUTER_BEVEL_RADIUS);
            translate([HOLE_RECT_X-OUTER_BEVEL_RADIUS+EDGE_OUTSET, OUTER_BEVEL_RADIUS-EDGE_OUTSET]) circle(OUTER_BEVEL_RADIUS);
            translate([HOLE_RECT_X-OUTER_BEVEL_RADIUS+EDGE_OUTSET, HOLE_RECT_Y-OUTER_BEVEL_RADIUS+EDGE_OUTSET]) circle(OUTER_BEVEL_RADIUS);
            translate([OUTER_BEVEL_RADIUS-EDGE_OUTSET,HOLE_RECT_Y-OUTER_BEVEL_RADIUS+EDGE_OUTSET]) circle(OUTER_BEVEL_RADIUS);
        }
    }
}

module ScrewHoles() {
    union() {
        circle(HOLE_RADIUS);
        translate([HOLE_RECT_X/2,0]) circle(HOLE_RADIUS);
        translate([HOLE_RECT_X,0]) circle(HOLE_RADIUS);
        translate([HOLE_RECT_X,HOLE_RECT_Y]) circle(HOLE_RADIUS);
        translate([HOLE_RECT_X/2,HOLE_RECT_Y]) circle(HOLE_RADIUS);
        translate([0,HOLE_RECT_Y]) circle(HOLE_RADIUS);
    }
}

module CherrySwitchSlot() {
    square([SWITCH_SLOT_WIDTH, SWITCH_SLOT_HEIGHT]);
}

module ReplaceableCherrySwitchSlot() {
    union() {
        CherrySwitchSlot();
        translate([-SWITCH_INDENT_DEPTH, SWITCH_INDENT_HEIGHT_OFFSET]) square([SWITCH_SLOT_WIDTH+SWITCH_INDENT_DEPTH*2,SWITCH_INDENT_WIDTH]);
        translate([-SWITCH_INDENT_DEPTH, SWITCH_SLOT_HEIGHT-SWITCH_INDENT_WIDTH-SWITCH_INDENT_HEIGHT_OFFSET]) square([SWITCH_SLOT_WIDTH+SWITCH_INDENT_DEPTH*2,SWITCH_INDENT_WIDTH]);
    }
}

module SwitchHoleLayout() {
    union() {
        for(y=[0:4]) {
            for(x=[0:13]) {
                if(!(y == 0 && x > 3 && x < 10)) {
                    translate([SWITCH_DELTA_X*x, SWITCH_DELTA_Y*y]) ReplaceableCherrySwitchSlot();
                }
                else if (x == 4 || x == 7) {
                    translate([SWITCH_DELTA_X*x+7.14375, SWITCH_DELTA_Y*y]) ReplaceableCherrySwitchSlot();
                }
                else if (x == 6 || x == 9) {
                    translate([SWITCH_DELTA_X*x-2.38125, SWITCH_DELTA_Y*y]) ReplaceableCherrySwitchSlot();
                }
            }
        }
    }
}

module TopPlate() {
    difference() {
        PlateOutline(0);
        translate([EDGE_OUTSET, EDGE_OUTSET]) union() {
            ScrewHoles();
            translate([SWITCH_BORDER_X,SWITCH_BORDER_Y]) SwitchHoleLayout();
        }
    }
}

TopPlate();