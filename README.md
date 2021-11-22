# 프로젝트명

recycling box

# 프로젝트 팀 구성원

- 우경희(pm)

- 조종혁

- 윤현

# 프로젝트 개요

- 전세계적으로 회자되고 있는 환경호보, 이를 위해선 무엇보다 환경보호에 대한 관심이 우선적으로 필요하다고 생각합니다. 따라서 사람들에게 환경보호의 관심을 이끌수도 있고 흥미를 느낄수도 있을 방법을 찾던 중, 만약 분리수거를 알아서 해주는 시스템이 있다면 어떨까해서 recycling box라는 주제를 선택하게 되었습니다.

- 이 recycling box의 주요 기능은 객체 인식을 통하여 분리수거가 가능한 페트병인지,알루미늄 캔인지 구분할 수 있는 기능이 있고, 부가적인 기능은 recycling box 가 꽉차있을때 fcm push 메시지를 전송하여 관리자가 확인할수 있도록 구성하였습니다. 

# 개발 환경

- 언어 : C/C++, javascript, java

- os : ubuntu

- library : Opencv, filesystem library(c++17), cpplinuxserial library, com.c

- tools : vscode, android studio, vim, Cmake

- framework : gstreamer, nodejs


# HW 구성

- jetson nano board

- AVR칩을 탑재한 보드

- 센서 및 액츄에이터
  
  - control servo
    
  - ultrasonic 
  
  - buzzer
  
  - neo_pixel

# SW 구성

- project main func

  - find pet object : matchTemplate

  - find can object : masking -> findcontour -> minAreaRect & approxPolyDP 

  - find label : masking -> morphology ->labeling

  - define filepath : filesystem

- project sub func

  - push alarm : android studio and fcm

- firmware
  
  - control servo control
    
  - ultrasonic control
  
  - buzzer control
  
  - neo_pixel control

- data communitation
  
  - serial communication cpp system headers
    
  - server : nodejs
  
# 프로젝트 작업 과정

### 소요기간 약 6주

- 프로젝트 주제 정하기

- 프로젝트 기능 정하기

- 프로젝트 파트리스트 정하기

- main 기능 구현하기

- 부가 기능 구현하기

# 외관 

![dwd](https://user-images.githubusercontent.com/88933098/142580963-b0937043-494c-4f16-8e1f-14169b578483.png)

# 동작영상

### opencv를 활용한 재활용 가능 pet,can 구분 구현 영상 (이미지 클릭후 영상으로 이동)
<br/>

[![Video Label](https://user-images.githubusercontent.com/88933098/142602186-7dcbbf48-1478-4d7e-848d-7c469a828f73.jpg)](https://youtu.be/2ykl-l5aJDE)

### recycling box 구현 영상 (이미지 클릭후 영상으로 이동)
<br/>

[![Video Label](https://user-images.githubusercontent.com/88933098/142602428-0d3db418-cd50-4137-952a-ded6b2d5be21.jpg)](https://youtu.be/o9Qvf0xHIZE)


# FLOWCHART(system)

![dwdwd](https://user-images.githubusercontent.com/88933098/142603105-f06b0f5e-0a40-499f-99ba-159e3b690738.png)


# 코드 리뷰 (Opencv)

![frfef](https://user-images.githubusercontent.com/88933098/142838223-159f460b-5c71-40a5-80d5-3b9ea32e0597.png)

## 1. find pet object : matchTemplate

![1](https://user-images.githubusercontent.com/88933098/142357606-de9da55c-f217-4e12-8ecc-d00a86a5258c.JPG)

~~~
//읽어온 이미지와 datasheet sample image를 비교하는 부분
for(int i = 0; i < SAMPLENUM; i++){
  	matchTemplate(subImg_, matReadVector[i], resVecotr[i], TM_CCOEFF_NORMED);
	normalize(resVecotr[i], res_normVector[i], 0, 255, NORM_MINMAX, CV_8U);
	minMaxLoc(resVecotr[i], 0, &maxvVector[i], 0, &maxlocVector[i]);
}
//유사도를 비교하는 부분
if(maxvVector[0] > 0.7){
	serialPort1.Write("N");
	cout << "CAP" <<endl;
	break;
}
 ~~~
 
 ## 2. find label : masking -> morphology ->labeling
![2](https://user-images.githubusercontent.com/88933098/142357629-9b73971e-ac28-4ed7-aae2-8cadf01b4fee.JPG)


1. 트랙바를 사용하여 upper & lower BGR 값을 찾기
https://github.com/woogyeonghee/libraries/blob/main/opencv/04.upper%26lower%20BGR%20Masking/main.cpp

2. masking
~~~
//트렉바를 사용하고 얻은 upper & lower BGR 값을 inRange를 통해 masking
Scalar lowerb(lower_hue_B, lower_hue_G, lower_hue_R);
Scalar upperb(upper_hue_B, upper_hue_G, upper_hue_R);
inRange(src_hsv, lowerb, upperb, mask);
~~~

3. morphology
~~~
// 모폴로지 연산을 통해 마스킹된 이미지를 열기, 닫기 연산 진행 
int morph_size = 2;
Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1),
Point(morph_size, morph_size));
morphologyEx(blue_mask, mask, MORPH_OPEN, element);
morphologyEx(blue_mask, mask, MORPH_CLOSE, element);
~~~

4. 레이블링을 사용하여 특정 범위 이상의 area 검출
~~~
numOfLabels = connectedComponentsWithStats(blue_mask, img_labels, stats, centroids, 8, CV_32S);
for (j = 1; j < numOfLabels; j++) {
	int area = stats.at<int>(j, CC_STAT_AREA);
	int left = stats.at<int>(j, CC_STAT_LEFT);
	int top = stats.at<int>(j, CC_STAT_TOP);
	int width = stats.at<int>(j, CC_STAT_WIDTH);
	int height = stats.at<int>(j, CC_STAT_HEIGHT);

	int centerX = centroids.at<double>(j, 0);
	int centerY = centroids.at<double>(j, 1);
	//area값이 1000이상일 때 rectangle,putText
	if (area > 1000) {
		rectangle(img, Point(left, top), Point(left + width, top + height),Scalar(0, 0, 255), 1);
		putText(img, to_string(area), Point(left, top), FONT_HERSHEY_PLAIN, 2.0,Scalar(255, 0, 0), 1);
	}
}
~~~
## 3. find ellipse : masking -> findcontour -> minAreaRect & approxPolyDP
![3](https://user-images.githubusercontent.com/88933098/142357693-31035f8c-ff64-4e6f-b780-003b7c016580.JPG)

1. 트랙바를 사용하여 upper & lower BGR 값을 찾기
https://github.com/woogyeonghee/libraries/blob/main/opencv/04.upper%26lower%20BGR%20Masking/main.cpp

2. findcontour
~~~
inRange(gray, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), gray);
//이때 찾고 싶은 object의 색상은 흰색 배경은 검정색으로 처리되어야 한다
findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
~~~

3. minAreaRect 주어진 점을 감싸는 최소크기의 회전된 사각형 반환
~~~
for (auto i : contours) {
        moment1 = moments(i, 1);
        x1 = int(moment1.m01 / moment1.m00);
        y1 = int(moment1.m10 / moment1.m00);

        RotatedRect temp = minAreaRect(Mat(i));
        //RotatedRect temp = fitEllipse(contours);

        if (i.size() > 200 && i.size() < 300) {
		//반환된 사각형의 가로 세로 정보로 타원 생성
        	ellipse(src, temp, Scalar(0, 0, 255), 3);
        	int he=temp.size.height;
        	int wi=temp.size.width;
		// 타원 구하는 공식 
        	area2 = he * wi * 0.5 * 0.5 * 3.14;
            	cout << area2 << endl;
	}
}
~~~

5. approxPolyDP를 사용해 외곽선 근사화 및 Area 측정
~~~
    	for (int i = 0; i < contours.size();i++) {
        	int area = contourArea(contours[i]);
        	if (area > 11100)
        	{
            	float peri = arcLength(contours[i], true);
            	approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
            	drawContours(src, conPoly, i, Scalar(255, 0, 0), 3);
            	cout << area << endl;
            	area3 = area;
        	}
	}
~~~

6. 두 상태의 area값 비교 후 유사 타원형인지 구분



# firmware (센서 및 액츄에이터 제어)

![frfef](https://user-images.githubusercontent.com/88933098/142840155-66c375f4-d8a6-4a8c-a5ce-00272a8cb7b2.png)

## 반복문 구간
~~~
while(1) {
  if(flag==4){
    flag = Read_start_ch();

  }
  if(flag==0){
    flag=check_garbage_full();
  }
  
  if(flag==1){
    flag = check_obj();
  }
  
  if(flag==2){
    flag = read_obj_msg();
	
  }
}
~~~

## 불안정한 초음파 센서 값으로 인한 문제 해결법
~~~
// begin시에 각 초음파 센서의 레퍼런스 길이를 전역 변수에 저장하는 함수를 구현 
void set_ref_dist(int id){
    int sum = 0;
    
    for (int i = 0; i < 100; i++)
        sum = sum + get_dist(id);

    //100번 측정한값을 레퍼런스 값으로 저장    
    ref_dist[id] = sum / 100;
}
~~~

## 원하는 buzzer의 음계를 간단히 문자열로 구현 하기 위한 코드
~~~
const int notes[] = { 262, 294, 330, 349, 392, 440, 494, 680 };

void sound_buzzer(char* song){

	for (int i=0;i<strlen(song);++i){
		player(song[i],100);
	}	
}

void player(char ch, int tm)
{
	if(ch >= '1' && ch <= '8')  {
		tone (buzzer, notes[(ch-'0')-1]);
		delay(tm);
	}
	else if(ch == '0'){
		delay(tm);
	}
	else;
	
	noTone(buzzer);
}
~~~

## 센서 및 엑츄에이터 테스트 코드
- 코드는 아래의 링크를 참조 바랍니다.  
https://github.com/woogyeonghee/firmware



## 참조한 데이터 시트 
![323232](https://user-images.githubusercontent.com/88933098/142371754-4fa7f6f8-f2de-46ef-8aad-51fb72a83868.JPG)

