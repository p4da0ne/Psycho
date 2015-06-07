/*
********************************************************************
*                                                                  *
*                                                                  *
*                                                                  *
********************************************************************
*                                                                  *
*           Типы объектов АКБД "Сатурн"            *
*                                                                  *
*******************************************************************/

#ifndef SATURNSIGNTYPES_H
#define SATURNSIGNTYPES_H

	 typedef enum ObjectsTypes 
	 {
		FORMATIONS = 1, 			//формирования
		SPECIAL_CONDITIONS = 2,     //особые условия
		SMI_MEANS = 3, 				// средства СМИ
		FORMATIONS_MEANS = 4, 		// средства формирований
		GROUPS_MEANS = 5, 			// средства организаций
		REGIONS = 6,				// регионы
		PERSONNEL = 7,				// персоналии
		EVENTS = 8,					// события
		GROUPS = 9,					// организации
		SMI = 10					//СМИ
	} 
	SignObjectsTypes;
#endif  //  SATURNSIGNTYPES_H

