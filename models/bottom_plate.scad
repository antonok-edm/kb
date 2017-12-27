/* 
 * Bottom keyboard plate
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

module BottomPlate() {
    difference() {
        PlateOutline(0);
        translate([EDGE_OUTSET, EDGE_OUTSET]) ScrewHoles();
    }
}

BottomPlate();