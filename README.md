# [드론 자동 착륙 장치]
- ## **착륙지의 AI 객체인식 시스템을 통해 호버링 중인 드론을 자동 착륙 시키는 시스템**

---

## 1. Project Summary (프로젝트 요약)
-   **개발 기간:** [2024.12.01] ~ [2025.05.21]
-   
-   **참여자   :** 김교원
-   
-   **주요 성과:**
    - 프로젝트 전 주기 경험
    - MissionPlanner 학습
    - 드론 HW 구조 학습
    - Colab 환경에서의 Yolo 모델 학습 방법 학습
    - **캡스톤 경진대회 장려상**

## 2. Tech Stack & Implementation (기술 스택 및 구현 핵심)

### 🛠️ 사용 기술
| Category | Skills |
| :--- | :--- |
| **Language** | <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" /> <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white" /> |
| **AI/Vision** | <img src="https://img.shields.io/badge/YoloV8n-FF5733?style=for-the-badge&logo=yolo&logoColor=white" /> <img src="https://img.shields.io/badge/Google%20Colab-F9AB00?style=for-the-badge&logo=googlecolab&logoColor=white" /> |
| **Dev Boards** | <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" /> <img src="https://img.shields.io/badge/Raspberry%20Pi-A22846?style=for-the-badge&logo=raspberrypi&logoColor=white" /> |
| **Tools & Platforms** | <img src="https://img.shields.io/badge/Mission%20Planner-3B5998?style=for-the-badge&logo=windows&logoColor=white" /> |

### 💡 구현 핵심 (F/W, SW 설계 포인트)

-   **실시간 처리:**
    > Interrupt 기반으로 위급 상황 시 일시 정지.
    > 
  
-   **안정성 확보:**
    >
    >
-   **주요 구조:**
    <img width="1438" height="815" alt="image" src="https://github.com/user-attachments/assets/a125bd92-3f40-4f59-853c-c70e11a120a1" />


## 3. Problem Solving & Learnings (문제 해결 및 배운 점)

-   **겪었던 문제:**
    > HW 셋업, AI 비전 인식 성능 향상, 테스트 시 인력 문제
    
-   **해결 과정:**
    > 시간이 오래 걸리더라도 이후 테스트를 위해 최대한 공식 사이트에서 제공된 가이던스를 따랐습니다.
    > 성능 향상을 위해 다각도의 데이터셋 추가, 학습량을 늘리며 실제 성능(주관적, 객관적 지표)이 고점을 찍은 시점에서 학습 중단, 카메라 모듈 (자동초점기능) 성능 고려하여 선정.
    
-   **배운 점 / 느낀 점:**
    > 개인 단위에서 어느정도 규모 있는 개발을 설계부터 자재 구매 이후 테스트 까지 전 과정을 겪어볼 수 있어서 좋았습니다.
    > 


---
