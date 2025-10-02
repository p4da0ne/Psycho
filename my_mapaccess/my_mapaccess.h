#ifndef MY_MAPACCESS_H
#define MY_MAPACCESS_H
#include "maptype.h"
#include <QObject>

#ifndef config_my_mapaccess_h
	#include <config_my_mapaccess.h>
#endif

#ifndef MAPAPI_H
	#include <mapapi.h>
#endif


class __EXPORT_MY_MAPACCESS MyMapAccess : public QObject
{
    Q_OBJECT
public:
    MyMapAccess();
    ~MyMapAccess();
	int drawContents( HMAP hMap, BITMAPINFOHEADER *lpDibInfo, char *lpBits, RECT *RectDraw);
	HMAP mapOpen(  const char * name, int val);
	// C������ ������ ������ ��������� �����
	HOBJ mapCreateObject(HMAP hMap,long int val1=1,long int kind=IDDOUBLE2,long int val2=0);
	// ��������� ����� ����� ������ � ������
	long int mapGetListCount(HMAP hMap);
	// ��������� ��� ������������� ����� �� ����� �����
	long int mapGetRscByName(const char * name,  char * rscname,  long int size);
	// ��������� ������������� ���������� ��������� �����
	double mapGetMapX1(HMAP hMap);
	double mapGetMapY1(HMAP hMap);
	double mapGetMapX2(HMAP hMap);
	double mapGetMapY2(HMAP hMap);
	// ��������� ���������� ��� (������) �������
	long int mapRscObjectCode(HMAP hMap, long int excode,long int local);
	// ��������� ����� ����� �� �����
	long int mapGetLayerCount(HMAP hMap);
	 // ��������� ��� ����������� �������
	long int mapRscObjectLocalInLayer(HMAP hMap, long int layer,long int number);
	 // ��������� �������� ������������� �������������� �������
	const char * mapRscSemanticName(HMAP hMap,long int code);
	 // ������������ �������� ������ ������� (!!!)
    long int mapRegisterObjectByKey(HOBJ hObj, const char * name_ff);
	//�������� ���������� �������
	long int mapAppendPointPlane(HOBJ hObj, double x,double g,long int val=0);
	long int mapCommitWithPlace(HOBJ hObj);
	long int mapSetBright(HMAP hmap, long int bright);
	void mapFreeObject(HOBJ hObj);
	void mapGetPictureSize(HMAP hMap,long int *mapW, long int *mapH);
	  // �������� ������ ����
	HIMAGE mapCreateImage(HWND hwnd);
	 // ������� ����� ����� (����� ��������� �����)
	HMAP mapCreateMap(const char * mapname, const char * rscname,MAPREGISTER * map,LISTREGISTER * sheet);
	HMAP mapCreateMapEx(const char * mapname, const char * rscname, MAPREGISTEREX * mapreg, LISTREGISTER * listreg);
	void mapCloseData(HMAP hMap);
	// ������� ���������������� �����
	HMAP mapCreateSite(const char * mapname, const char * rscname,CREATESITE * createsite);
	HMAP mapCreateSiteEx(const char * mapname,const char * rscname,CREATESITEEX * createsite);
	HMAP mapCreateSiteUn(const WCHAR * mapname, const WCHAR * rscname,CREATESITEUN * createsite);


	//�������� ������� ����������� ���������������� �����
	long int mapSetSiteViewOrder(HMAP hMap, long int number,long int order);

	//�������� ����� ���������������� ����� � �������
	long int mapGetSiteNumber(HMAP hMap,HSITE hSite);

	//������� ���������������� �����
	HMAP mapOpenData(const char * name, long int mode = 0);
	 // ��������� - ����� �� ����� ���������������
	long int mapGetSiteEditFlag(HMAP hMap, HSITE hSite);
	// ���������� ���� �������������� ����� (0 - �� �������������)
	long int mapSetSiteEditFlag(HMAP hMap,HSITE hSite, long int flag);
	// ������� ���������������� ����� �� �������� �����
	HSITE mapCreateAndAppendSiteUn(HMAP hMap,  const WCHAR *  mapname,const WCHAR * rscname, CREATESITEUN * createsite);
	HSITE mapCreateAndAppendSite(HMAP hMap, const char * mapname,const char * rscname, CREATESITE * createsite);
	// ������� ���������������� ����� � �������� ������ �����
	HSITE mapCloseSiteForMap(HMAP hMap, HSITE hSite);
	HSITE mapCloseSiteForMapByName(HMAP hMap,const char * name);



	// ���������� ���������� ������ � ����������� �� ���� �����
	long int mapRegisterFromMapType(int maptype, MAPREGISTEREX * mapreg);
	//���������� ������� �� ����� MAP, SIT, MTW, MTQ, RSW, MPT
		long int mapAppendDataUn(HMAP hMap, const WCHAR * name, long int mode=0);
		long int mapAppendData(HMAP hMap, const char * name, long int mode=0);
		HOBJ  mapCreateSiteObject(HMAP hMap,HSITE hSite, long int kind = IDDOUBLE2, long int text = 0);
	//�������� � ������������ � �����
		 HSITE mapOpenSiteForMap(HMAP hMap, const char * sitename, long int mode);
	 // ��������� ����� ����� �� �����
		 long int mapGetSiteLayerCount(HMAP hMap,  HSITE hSite);
		 // ��������� �������� ������������� �������������� �������
		 double mapSemanticDoubleValue(HOBJ info, long int number);
		 // ��������� �������� ������������� �������������� �������
		 double mapSemanticCodeDoubleValue(HOBJ info, long int code, long int number);
	//�������� ������������ ��������
		long int mapCheckAndUpdate(MAPREGISTEREX * mapreg, LISTREGISTER * listreg, long int priority);
	//��� ������� ����� � �������
		const char * mapGetMainName(HMAP hMap);
	// ���������� ������� ��������� �������� �� ����������
		void mapSetTotalSelectFlag(HMAP hMap, long int flag=-1);
	// ���������� ������� ����������� (����������� ��������)
		long int mapSetViewScale(HMAP hMap, long int * x, long int * y, float scale);
		 // ��������� ����������� ������� ����������� �����
		long int mapGetShowScale(HMAP);
	//����� �������
		//��� ������
		void mapChangeViewScale(HMAP hMap, long int *x, long int *y, float Change);
		long int mapGetViewSelect(HMAP hMap, HSELECT select);	
		HSELECT mapCreateMapSelectContext(HMAP hMap);
		//�����
		HOBJ mapWhatObject(HMAP hMap, HOBJ info, MAPDFRAME * frame, long int flag = WO_LAST, long int place = PP_MAP);
		HOBJ mapWhatObjectBySelect(HMAP hMap, HOBJ info, MAPDFRAME * frame, HSELECT select, long int flag = WO_LAST, long int place = PP_MAP);
	 // �������� �� ����� �������, �������������� �������� �������
		void mapViewSelect(HMAP hMap, HWND hWnd, DOUBLEPOINT * point, HSELECT select, COLORREF color, long int place = PP_MAP);
	  // ������� �������� (�������� �������) ������/�����������
		void mapDeleteSelectContext(HSELECT select);
		//������� � ���������� � ���������
		void mapPictureToMap(HMAP hMap,double *x, double *y);
		//�� ��������� � �����
		void mapMapToPlane(HMAP hmap, double * x, double *y);
		//������� � ���������� � ������
		void mapPictureToPlane(HMAP hMap, double * x, double * y);
	// ��������� �������� ������� �� ����������� ������ (number)
		const char * mapRscObjectNameInLayer(HMAP hmap,long int layer,long int number);
		const char * mapObjectName(HOBJ info);
	// ��������� ����������������� ��� �������
	long int * mapRscObjectExcodeInLayer(HMAP hMap,long int layer,long int number);
	// ��������� - ���� �� �����-���� �������� ������
	long int mapIsActive(HMAP hMap);
	// ��������� ��� ������ ��������� �������� ������� � ������
	long int mapGetAccessError();
	// ��������� ���������� ����� �������
	long int mapObjectKey(HOBJ info);
	 // ��������� �������� ���� ������� ("Layer" = "Segment")
	const char * mapSegmentName(HOBJ info);
	// ��������� ������� �������� �������������� ���� ��� ������ ���������
	HWND mapGetHandleForEvent(HMAP hMap);
	// ��������� ������������� �������������� �����
	 HRSC mapGetRscIdent(HMAP hMap,HSITE hSite);
	 // ��������� ������������� �������������� �����, ���������� ������
	 HRSC mapGetRscIdentByObject(HOBJ info);
	 // ��������� ���������� ����� ��������, ������� ��� ����� ����
	 long int mapAvailableSemanticCount(HOBJ info);
	  // ��������� ������� ��� ��������� ��������� �� ������
	 long int mapAvailableSemanticCode(HOBJ info,int number);
	 // �������� ����� �������������� � ��������� �������
	 long int mapAppendSemanticDouble(HOBJ info,long int code, double value);
	 long int mapAppendSemantic(HOBJ info,long int code, const char * value, long int size);
	 // ��������� �������� ������������� �������������� �������
	  long int  mapSemanticValue(HOBJ info, long int number,  char * value,  long int size);
	  long int	mapSemanticCodeValue(HOBJ info,long int code, char * place,long int maxsize,long int number);
	 // �������� ����������� ��������� ��������� ����� � ������ ������ 
	 long int mapDrawImageMap(HIMAGE himage, HMAP hMap, RECT * rect,  POINT * position);
	 // ����������/��������/��������� ���������������� �������
	long int mapSetObjectScale(HOBJ info, long int scale);
	// ���������/���������� �������� ��������� ��������� ������� (���� 4 �-���)
	long int mapObjectTopScale(HOBJ info);
	long int mapSetObjectTopScale(HOBJ info, long int scale);
	long int mapObjectBotScale(HOBJ info);
	long int mapSetObjectBotScale(HOBJ info, long int scale);
	// ����������/��������/��������� ������� "�� �������" �������
	long int mapSetObjectPress(HOBJ info, long int press);
	long int mapGetObjectPress(HOBJ info);
	// ���������� ������ ������ ����� ��������� � ������ ������
	long int mapDrawImageMapObject(HIMAGE himage, HMAP hMap, PAINTPARM * parm, HOBJ object);
	// �������� ���������� �������
	long int mapClearObject(HOBJ info,  long int sheetnumber = 1,  long int kind = IDFLOAT2);
	// ������������ �������� ������ ������� (!!!)
	long int mapRegisterObject(HOBJ info, long int excode, long int local);
	long int mapDescribeObject(HOBJ info, long int code);
	// ��������� ����������������� ��� �������
	long int mapObjectExcode(HOBJ info);
	 // ��������� ������� ����� (�� -16 �� +16)
	long int mapGetBright(HMAP hMap);
// ��������� ������������� (�� -16 �� +16)
	long int	mapGetContrast(HMAP hMap);
  // ���������� ������������� (�� -16 �� +16)
	long int	mapSetContrast(HMAP hMap, long int contrast);
  // ��������� ������������� ������� ��������� ���       // 21/07/06
	long int	mapGetIntensity(HMAP hMap);
  // ���������� ������������� ������� ��������� ���
	long int	mapSetIntensity(HMAP hMap, long int intensity);
	// ��������� �������� ������ ����
	HDC mapGetImageDC(HIMAGE hImage);
	 // ������� ����� ����� � ������ ��� ���������� �������  
	HIMAGE mapCreateImageEx(long int width, long int height);
	 // ���������� �������� ����� �� �������� ����������
	void mapPaint95(HMAP hMap, HDC hdc,  long int erase, RECT * rect);
	  // ���������� ������������� ���� ��� ������ ���������
	HWND mapGetHandleForMessage();
	 // ������� ����������� ����� � Image (������)
	long int	mapPaintToImage(HMAP hMap,  RGBQUAD * palette,  char * lpImage,  long int width, long int height,  RECT * rect);
	//------------------------------------------------------------
	// �������������� �� ������ �� ��������� (�������� �����) � ������������� ���������� � �������� (���������� ��������� WGS84)
	long int mapPlaneToGeoWGS84(HMAP hmap,double *Bx, double *Ly);
	long int mapPlaneToGeoWGS843D(HMAP hmap,double *Bx, double *Ly, double *H);
	 // �������������� �� ��������� �� ����� (������ �����) � ������� �� �����������
	void mapMapToPicture(HMAP hMap, double * x, double * y);
	//  �������������� �� ������������� ��������� � ��������
  long int mapGeoWGS84ToPlane3D(HMAP hmap, double *Bx, double *Ly, double * H);
   // ������ - �������������� �� �������� � ������������� ����������� �� ������� ������������� � �������
	long int mapIsGeoSupported(HMAP hmap);
	// �������������� ��������� �� �������� � �������
	void mapDegreeToRadian(GEODEGREE * degree, double * radian);
	 // �������������� �� ������������� ��������� � ��������  � ����� �� ��������� � ������������ � ��������� �����
	long int mapGeoToPlane(HMAP hmap,double *Bx, double *Ly);
	// �������������� �� ������ �� ��������� (�������� �����) � ������������� ���������� � �������� (��������� �����������)
	long int mapPlaneToGeo42(HMAP hmap,double *Bx, double *Ly);
	long int mapPlaneToGeo423D(HMAP hmap,double *Bx, double *Ly, double *H);
	 // �������������� ��������� �� ������ � �������
	void	 mapRadianToDegree(double * radian,  GEODEGREE * degree);
	 // �������������� �� ������ �� ��������� � �������� �� ����� (������ �����)
	void	 mapPlaneToMap(HMAP hMap,double * x, double * y);
	HPRINTER prnLoadPrinter();
	// �������� �������� ���� ������������� �������������� �������
	long int	mapSetSemanticCode(HOBJ info, long int number, long int code);
 // �������� �������� ������������� �������������� �������
	long int	mapSetSemanticValue(HOBJ info, long int number, char * place, long int maxsize);
	
	//�����! ! ! ! ! ! ! ! ! ! ! ! ! !
	//long int _MAPAPI mapOpenRstForMap(HMAP hMap, const char * rstname, long int mode);
	 long int  openRstForMap(HMAP hMap, const char * rstname, long int mode);
	 long int  closeRstForMap(HMAP hMap, long int number);
	 // ���������/���������� ������� ����������� ������ ��� ������
	 long int  setRstViewOrder(HMAP hMap, long int number, long int order);
protected:
    

public:
	HINSTANCE LibInst;
	
};



#endif
