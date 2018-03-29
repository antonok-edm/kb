/* 
 * Keyboard midsection
 * Author: Anton Lazarev
 * https://github.com/antonok-edm/kb
 *
 * Units are in millimeters.
 */

/*
 * IMPORTANT:
 *
 * This model imports a DXF file of the PCB's outline.
 * This can be created as follows:
 *   - Open kb/pcb/kb-pcb.kicad_pcb in Kicad's Pcbnew editor
 *   - Select File > Plot
 *   - Make sure Edge.Cuts is the only selected layer
 *   - Uncheck "Plot all layers in outline (polygon) mode"
 *   - Ensure that "Plot Format" is set to DXF
 *   - Set "Output directory" to "../models/"
 *   - Check "Use auxiliary axis as origin"
 */

//Setting $fn to 400 creates very smooth curves, but takes a few
//minutes to render.
//Setting $fn to a lower value is recommended during development.
$fn=15;
HEIGHT=16;
CORNER_RADIUS=5;
BEVEL_RADIUS=1;

WIDTH=280.7;
DEPTH=109.25;

PLATE_XY_TOLERANCE=.2;

PLATE_HEIGHT=1.5;

HOLE_RECT_X=271.7;
HOLE_RECT_Y=100.25;
OUTER_BEVEL_RADIUS=3;
HOLE_RADIUS=1.5;
EDGE_OUTSET=2.5;

UPPER_LIP_WIDTH=1.8;

PORT_HEIGHT=5.6;
PORT_WIDTH=9.3;
PORT_BEVEL_RADIUS=0.5;

PORT_CENTER_TO_PCB_EDGE=29.718;
PORT_BOTTOM_DZ=4.8;
PCB_WIDTH=268.986;
PCB_HEIGHT=97.536;
PCB_INSET=(WIDTH-PCB_WIDTH)/2;
PCB_XY_TOLERANCE=1.1;

PORT_INSET_LEFT=21.5;
PORT_INSET_WIDTH=28;
PORT_INSET_FRONT=107.433;
PORT_INSET_SUPPORT_DEPTH=1.1615;
UPPER_PLATE_SUPPORT_HEIGHT=2;

//Reference variables
PLATE_X_DIM=HOLE_RECT_X+EDGE_OUTSET*2;
PLATE_Y_DIM=HOLE_RECT_Y+EDGE_OUTSET*2;

module CornerColumn() {
    translate([0,0,HEIGHT/2]) rotate_extrude() {
        hull() {
            translate([0,-HEIGHT/2]) square([CORNER_RADIUS-BEVEL_RADIUS,HEIGHT]);
            translate([CORNER_RADIUS-BEVEL_RADIUS,-(HEIGHT/2-BEVEL_RADIUS)]) circle(BEVEL_RADIUS);
            translate([CORNER_RADIUS-BEVEL_RADIUS,HEIGHT/2-BEVEL_RADIUS]) circle(BEVEL_RADIUS);
        }
    }
}

module SolidBody() {
    translate([CORNER_RADIUS,CORNER_RADIUS,0]) hull() {
        CornerColumn();
        translate([WIDTH-CORNER_RADIUS*2,0,0]) CornerColumn();
        translate([WIDTH-CORNER_RADIUS*2,DEPTH-CORNER_RADIUS*2,0]) CornerColumn();
        translate([0,DEPTH-CORNER_RADIUS*2,0]) CornerColumn();
    }
}

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

module MiniUSBPort() {
    translate([PORT_BEVEL_RADIUS, PORT_BEVEL_RADIUS]) hull() {
        circle(PORT_BEVEL_RADIUS);
        translate([PORT_WIDTH-PORT_BEVEL_RADIUS*2,0]) circle(PORT_BEVEL_RADIUS);
        translate([PORT_WIDTH-PORT_BEVEL_RADIUS*2,PORT_HEIGHT-PORT_BEVEL_RADIUS*2]) circle(PORT_BEVEL_RADIUS);
        translate([0,PORT_HEIGHT-PORT_BEVEL_RADIUS*2]) circle(PORT_BEVEL_RADIUS);
    }
}

difference() {
    SolidBody();
    // Make room for plates in the top and bottom
    translate([UPPER_LIP_WIDTH, UPPER_LIP_WIDTH, 0]) linear_extrude(PLATE_HEIGHT) PlateOutline(PLATE_XY_TOLERANCE);
    translate([UPPER_LIP_WIDTH, UPPER_LIP_WIDTH, HEIGHT-PLATE_HEIGHT]) linear_extrude(PLATE_HEIGHT) PlateOutline(PLATE_XY_TOLERANCE);
    // Screw holes
    linear_extrude(HEIGHT) translate([UPPER_LIP_WIDTH+PLATE_XY_TOLERANCE+EDGE_OUTSET,UPPER_LIP_WIDTH+PLATE_XY_TOLERANCE+EDGE_OUTSET]) ScrewHoles();
    // Rebuild geometry near USB port area
    difference() {
        // PCB-shaped hole with tolerance
        union() {
            /* This line requires the PCB Edge.Cuts layer to be exported. */
            linear_extrude(HEIGHT) offset(r=PCB_XY_TOLERANCE) translate([PCB_INSET,PCB_INSET]) import(file = "../models/kb-pcb-Edge.Cuts.dxf");
            /* If you are getting an error here, please read the "IMPORTANT" section at the beginning of this file. */
            
            // Fill in the small hole within the boundaries of the the PCB
            linear_extrude(HEIGHT) translate([PORT_INSET_LEFT,98]) square([PORT_INSET_WIDTH,8]);
        }
        // Fill in the outer wall that was removed
        linear_extrude(HEIGHT) translate([PORT_INSET_LEFT,PORT_INSET_FRONT]) square([PORT_INSET_WIDTH,UPPER_LIP_WIDTH]);
        // Upper plate support
        translate([PORT_INSET_LEFT,PORT_INSET_FRONT-PORT_INSET_SUPPORT_DEPTH,HEIGHT-PLATE_HEIGHT-UPPER_PLATE_SUPPORT_HEIGHT])
            linear_extrude(UPPER_PLATE_SUPPORT_HEIGHT) square([PORT_INSET_WIDTH,UPPER_LIP_WIDTH]);
    }
    // Carve out USB port
    translate([PCB_INSET+PORT_CENTER_TO_PCB_EDGE-PORT_WIDTH/2,PORT_INSET_FRONT,PORT_BOTTOM_DZ])
        mirror([0,1,0]) rotate([90,0,0]) linear_extrude(EDGE_OUTSET) MiniUSBPort();
}
