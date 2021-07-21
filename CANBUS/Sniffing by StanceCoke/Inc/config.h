/*
 * config.h
 *
 *  Created on: 30.07.2020
 *      Author: stancecoke
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define CALIB 1	 //better value not known :-)
#define FILTER 6 //average from 2^6 samples, sample rate = 50Hz
//predefine Display Types
#define DISPLAY_TYPE_KUNTENG (1<<1)							// For use with Kunteng LCD Displays
#define DISPLAY_TYPE_DEBUG (1<<0)							// For ASCII-Output in Debug mode);


//Select display
#define DISPLAY_TYPE DISPLAY_TYPE_DEBUG
#endif /* CONFIG_H_ */
