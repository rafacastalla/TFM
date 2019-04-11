/* AUTO-GENERATED FILE.  DO NOT MODIFY.
 *
 * This class was automatically generated by the
 * java mavlink generator tool. It should not be modified by hand.
 */

package com.MAVLink.enums;

/** 
* Focus types for MAV_CMD_SET_CAMERA_FOCUS
*/
public class SET_FOCUS_TYPE {
   public static final int FOCUS_TYPE_STEP = 0; /* Focus one step increment (-1 for focusing in, 1 for focusing out towards infinity). | */
   public static final int FOCUS_TYPE_CONTINUOUS = 1; /* Continuous focus up/down until stopped (-1 for focusing in, 1 for focusing out towards infinity, 0 to stop focusing) | */
   public static final int FOCUS_TYPE_RANGE = 2; /* Zoom value as proportion of full camera range (a value between 0.0 and 100.0) | */
   public static final int SET_FOCUS_TYPE_ENUM_END = 3; /*  | */
}
            