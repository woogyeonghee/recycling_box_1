# 프로젝트명

recycling box

# 프로젝트 팀 구성원

- 우경희(pm)

- 조종혁

- 윤현

# 프로젝트 개요

- 전세계적으로 회자되고 있는 환경호보, 이를 위해선 무엇보다 환경보호에 대한 관심이 우선적으로 필요하다고 생각합니다. 따라서 사람들에게 환경보호의 관심을 이끌수도 있고 흥미를 느낄수도 있을 방법을 찾던중, 만약 분리수거를 알아서 해주는 로봇이 어떨까해서 recycling box라는 주제를 선택하게 되었습니다.

- 이 recycling box의 주요 기능은 객체 인식을 통하여 분리수거가 가능한지 페트병인지, 캔인지 구분할 수 있는 기능이 있고, 부가적인 기능은 recycling box 가 꽉차있을때 fcm push 메시지를 전송하여 관리자가 확인할수 있도록 구성하였습니다. 

# HW 구성

- jetson nano board

- AVR칩을 탑재한 보드


# SW 구성

- project main func

  - pet object : matchTemplate

  - can object : masking -> findcontour -> minAreaRect & approxPolyDP 

  - find label : masking -> morphology ->labeling

  - define filepath : filesystem

- project sub func

  - push alarm : android studio and fcn

- firmware
  
  - control servo
    
  - ultrasonic 
  
  - buzzer
  
  - neo_pixel

- communitation
  
  - serial communication cpp system headers
    
  - server : nodejs
  
# 프로젝트 작업 과정

- 프로젝트 주제 정하기

- 프로젝트 기능 정하기

- 프로젝트 파트리스트 정하기

- main 기능 구현하기

- 부가 기능 구현하기


# 동작영상


{% includes 1.html id="https://www.youtube.com/embed/" %}
