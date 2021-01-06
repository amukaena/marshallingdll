# marshallingdll
C++ Dll and mashalling to C#


## 개요

* 리눅스에서 CPP로 so(DLL)파일 만들고 그 so를 marshalling해서 C#에서 불러다 쓸 수 있도록 함

* SystemController.cs 파일은 C# 프로젝트에 포함해서 사용한다.

* 그 외 파일들은 CPP DLL용 프로젝트 파일들이다.

## 구현된 기능
* reboot

* shell파일 경로 전달하면 해당 shell파일을 실행

* 네트워크 세팅 초기화
  * 초기화 되는 정보는 하드코딩되어있는 상태
    * static
    * ip - 172.16.160.60
    * netmask - 255.255.0.0
    * gateway - 172.16.100.1
  * 네트워크세팅이/etc/network/interfaces 파일을 다룰 때를 가정하여 구현되어있는 상태
    * 파일 내용이 아래와 동일한형식으로 구성되고 interface 이름이 eth0일 때를 기준으로 구현
      ```
      auto lo
      iface lo inet loopback

      auto eth0
      iface eth0 inet static
      address xxx.xxx.xxx.xxx
      netmask xxx.xxx.xxx.xxx
      gateway xxx.xxx.xxx.xxx
      ```
      
* 현재 네트워크 설정 값을 얻기
  ```
  typedef struct NetworkInfo
  {
    char dhcp[8]; // static, dhcp
    char ip[16];  // ip address
    char netmask[16]; // netmask
    char gateway[16]; // gateway
  }
  ```
  * 위 구조체를 기반으로 call by pointer로 현재 네트워크 설정 값들을 얻을 수 있다.
  * 단 interface 이름은 eth0 기준으로 구현

* 네트워크 설정 변경
  ```
  typedef struct NetworkInfo
  {
    char dhcp[8]; // static, dhcp
    char ip[16];  // ip address
    char netmask[16]; // netmask
    char gateway[16]; // gateway
  }
  ```
  * 위 구조체에 들어있는 값으로 eth0의 네트워크 설정을 변경한다.
  
* 현재 스피커 볼륨 얻어오기
  * 특정 장치 기준이라 일반적으로는 사용 못할 수 있다.
  
* 스피커 볼륨 조절하기
  * 특정 장치의 특정 세팅 기준이기에 일반적인 사용은 불가
  
* 장치의 disk, memory, cpu 사용량 및 mac 주소 얻기
  ```
  typedef struct ResourceInfo
  {
    long disktotal; // disk 전체 용량 [KB]
    long diskusage; // disk 현재 사용량 [KB]
    long memorytotal; // memory 전체 용량 [KB]
    long memoryusage; // memory 현재 사용량 [KB]
    long cputotal; // cpu 전체 가용량
    long cpuusage; // cpu 현재 사용량
    char mac[18]; // eth0 mac address
    char wirelessmac[18]; // wlan0 mac address
  }
  ```
  * 위 구조체를 기준으로 현재 시스템 정보들을 얻을 수 있다.

* K_Face_XT_API 라는 실행 명령을 포함한 프로세스 kill 기능

## 개선이 필요한 사항
* 하드코딩 되어있는 내용들을 파라미터로 받아서 처리할 필요가 있다.
* 특정 장치 기준으로 개발되고 테스트되어서 일반 리눅스에서 사용 못하는 기능들 수정 필요
