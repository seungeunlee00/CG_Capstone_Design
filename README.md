# 2022-2 컴퓨터그래픽스 설계

### 환경 설정
visual studio에서 프로젝트 생성  
bin, include, lib 폴더 복사해서 붙여넣기  
bin/x64/freeglut.dll 폴더 복사해서 붙여넣기  
Release x64로 변경  
속성 -> C/C++ -> 일반 -> 추가 포함 디렉터리 -> include;  
속성 -> 링커 -> 일반 -> 추가 라이브러리 디렉터리 -> lib\x64;  
속성 -> 링커 -> 입력 -> 추가 종속성 -> freeglut.lib;  
