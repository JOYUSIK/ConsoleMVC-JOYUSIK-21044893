# ConsoleMVC-JOYUSIK-21044893

MVC 패턴 구조 검증을 위한 PoC 프로젝트입니다.

## 목적

반도체 시료 생산주문관리 시스템(미션2) 구현에 앞서,
**Model / Repository / View / Controller** 레이어 분리가 올바르게 동작하는지 검증합니다.

## 환경

| 항목 | 내용 |
|---|---|
| 언어 | C++17 |
| 빌드 | CMake 4.3.1 + MSVC (Visual Studio 2025 Preview) |
| JSON | nlohmann/json 3.11.3 (빌드 시 자동 다운로드) |

## 프로젝트 구조

```
ConsoleMVC-JOYUSIK-21044893/
├── CMakeLists.txt
├── include/
│   ├── model/Item.h               # 순수 데이터 구조 (비즈니스 로직 없음)
│   ├── repository/ItemRepository.h # JSON 파일 CRUD
│   ├── controller/ItemController.h # 흐름 제어
│   ├── view/ItemView.h            # 콘솔 출력 + 입력
│   └── third_party/json.hpp       # nlohmann/json (빌드 시 자동 생성)
├── src/
│   ├── model/Item.cpp
│   ├── repository/ItemRepository.cpp
│   ├── controller/ItemController.cpp
│   ├── view/ItemView.cpp
│   └── main.cpp
└── data/                          # JSON 영속성 파일 (git 제외)
```

## 레이어 역할

| 레이어 | 역할 | 금지사항 |
|---|---|---|
| Model | 순수 데이터 구조 + JSON 직렬화/역직렬화 | 비즈니스 로직, I/O |
| Repository | JSON 파일 CRUD만 담당 | 비즈니스 판단, cout |
| Controller | 흐름 제어, Repository 호출 | 비즈니스 로직, cout |
| View | cout/cin 전담 | 비즈니스 로직, 파일 접근 |

## 빌드 및 실행

### Visual Studio에서 열기 (권장)
1. Visual Studio 실행
2. `파일 > 열기 > CMake...`
3. 이 폴더의 `CMakeLists.txt` 선택
4. `빌드 > 모두 빌드 (F7)`

### 터미널에서 빌드
```powershell
$cmake = "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

# 구성 (최초 1회)
& $cmake -S . -B build

# 빌드
& $cmake --build build --config Release

# 실행
.\build\Release\ConsoleMVC.exe
```

## 실행 화면

```
============================================================
 아이템 관리 시스템
============================================================
 [1] 아이템 등록
 [2] 아이템 목록 조회
 [3] 아이템 상세 조회
 [4] 아이템 수정
 [5] 아이템 삭제
 [0] 종료
============================================================
 선택 >
```
