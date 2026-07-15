# ConsoleMVC — PoC 1: MVC 패턴 구조 검증

반도체 시료 생산주문관리 시스템(Mission 2) 구현에 앞서,
**Model / Repository / View / Controller** 레이어 분리가 올바르게 동작하는지 검증하는 PoC입니다.

---

## 목차

1. [MVC 패턴이란?](#1-mvc-패턴이란)
2. [이 프로젝트의 레이어 구조](#2-이-프로젝트의-레이어-구조)
3. [각 레이어 코드 설명](#3-각-레이어-코드-설명)
4. [코드 동작 흐름](#4-코드-동작-흐름)
5. [프로젝트 구조](#5-프로젝트-구조)
6. [빌드 및 실행](#6-빌드-및-실행)

---

## 1. MVC 패턴이란?

MVC는 **Model · View · Controller** 세 역할로 코드를 분리하는 소프트웨어 설계 패턴입니다.

```
사용자
  │  입력
  ▼
Controller ──────► Model (데이터)
  │                   │
  │                   ▼
  │              Repository (저장소)
  │
  ▼
View (화면)
  │  출력
  ▼
사용자
```

### 왜 분리하나요?

| 문제 상황 | MVC 적용 후 |
|---|---|
| 화면 출력 코드와 DB 처리 코드가 한 파일에 섞임 | 각 역할이 독립 파일로 분리 |
| 한 줄 수정이 전체에 영향 | 해당 레이어만 수정 |
| 테스트가 불가능 | 레이어별 단독 테스트 가능 |

> **비유**: 식당으로 치면 손님(사용자) ↔ 웨이터(Controller) ↔ 메뉴판/재료(Model/Repository) ↔ 홀 디스플레이(View)

---

## 2. 이 프로젝트의 레이어 구조

이 프로젝트는 순수 MVC에 **Repository** 레이어를 추가한 구조입니다.

```
┌──────────────────────────────────────────┐
│  View  (ItemView)                        │
│  · cout / cin 만 담당                    │
│  · 화면 출력 + 사용자 입력 수집          │
└───────────────┬──────────────────────────┘
                │ 데이터 전달 (struct Item)
┌───────────────▼──────────────────────────┐
│  Controller  (ItemController)            │
│  · 흐름 제어만 담당                      │
│  · View 에서 받은 입력 → Repository 전달 │
│  · Repository 결과 → View 에 전달        │
└───────────────┬──────────────────────────┘
                │ CRUD 호출
┌───────────────▼──────────────────────────┐
│  Repository  (ItemRepository)            │
│  · JSON 파일 읽기/쓰기만 담당           │
│  · findAll / findById / save / update    │
└───────────────┬──────────────────────────┘
                │ 직렬화/역직렬화
┌───────────────▼──────────────────────────┐
│  Model  (Item)                           │
│  · 순수 데이터 구조 (struct)             │
│  · toJson() / fromJson() 만 포함         │
└──────────────────────────────────────────┘
```

### 레이어별 금지 규칙

| 레이어 | 역할 | 절대 하면 안 되는 것 |
|---|---|---|
| **Model** | 데이터 정의 + JSON 변환 | `cout`, 파일 읽기, 비즈니스 판단 |
| **Repository** | 파일 CRUD | `cout`, 비즈니스 판단 |
| **Controller** | 흐름 제어 | `cout`, 직접 파일 접근 |
| **View** | 입출력 전담 | 비즈니스 로직, 파일 접근 |

---

## 3. 각 레이어 코드 설명

### 3-1. Model — `Item`

```cpp
// ConsoleMVC/model/Item.h
struct Item {
    std::string id;           // "ITEM-001"
    std::string name;         // "실리콘 웨이퍼"
    std::string description;  // "12인치 300mm"
    int         quantity;     // 100
    std::string createdAt;    // "2026-07-15"

    nlohmann::json toJson() const;           // struct → JSON
    static Item fromJson(const nlohmann::json& j); // JSON → struct
};
```

- `struct`(구조체)로 선언 — 멤버 변수가 기본 public이므로 데이터 전달 객체(DTO)에 적합
- `toJson()` / `fromJson()`: 파일에 저장하거나 불러올 때 JSON ↔ struct 변환만 담당
- **비즈니스 로직이나 출력 코드는 전혀 없음**

---

### 3-2. Repository — `ItemRepository`

```cpp
// ConsoleMVC/repository/ItemRepository.h
class ItemRepository {
public:
    std::vector<Item>      findAll()  const;           // 전체 조회
    std::optional<Item>    findById(const std::string& id) const; // 단건 조회
    Item                   save(Item item);             // 등록
    bool                   update(const Item& item);    // 수정
    bool                   remove(const std::string& id); // 삭제
private:
    std::string dataPath_;    // "data/items.json"
    // 내부 헬퍼
    std::vector<Item> load()   const;                  // JSON 파일 → vector
    void              persist(const std::vector<Item>&) const; // vector → JSON 파일
    std::string       nextId(const std::vector<Item>&) const;  // "ITEM-001" 채번
};
```

**핵심 포인트 두 가지**

1. **`std::optional<T>`** — `findById`가 결과를 찾지 못했을 때 `nullptr` 대신 `std::nullopt`를 반환합니다. Controller에서 `if (!item)` 으로 안전하게 체크할 수 있습니다.

2. **ID 자동 채번** — 기존 아이템 중 최대 번호를 찾아 +1 합니다. 빈 상태이면 `"ITEM-001"`부터 시작합니다.

```cpp
// save() 내부 흐름
Item ItemRepository::save(Item item) {
    auto items = load();          // 파일에서 전체 읽기
    item.id = nextId(items);      // ID 자동 생성
    items.push_back(item);        // 목록에 추가
    persist(items);               // 파일에 전체 다시 쓰기
    return item;                  // 생성된 ID 포함해서 반환
}
```

---

### 3-3. View — `ItemView`

```cpp
// ConsoleMVC/view/ItemView.h
class ItemView {
public:
    std::string showMenu()    const;                         // 메뉴 출력 + 선택값 반환
    Item        inputItem()   const;                         // 등록 입력 수집
    std::string inputId(const std::string& action) const;   // ID 입력
    Item        inputUpdate(const Item& current)   const;   // 수정 입력 (기존값 유지 가능)

    void showItemList  (const std::vector<Item>& items) const;
    void showItemDetail(const Item& item)               const;
    void showSuccess   (const std::string& msg)         const;
    void showError     (const std::string& msg)         const;
    bool confirm       (const std::string& msg)         const; // Y/N 확인
};
```

- **모든 `cin` / `cout`이 이 클래스에만 존재**합니다. 다른 레이어에는 단 한 줄도 없습니다.
- `showMenu()`는 메뉴를 출력하고 사용자 입력값을 `string`으로 **반환**만 합니다. 분기 판단은 하지 않습니다.
- `inputUpdate()`는 Enter를 그냥 누르면 기존 값을 유지합니다 — View 안에서 처리하되 비즈니스 판단은 아님.

---

### 3-4. Controller — `ItemController`

```cpp
// ConsoleMVC/controller/ItemController.h
class ItemController {
public:
    void create();   // 등록 흐름
    void listAll();  // 목록 조회 흐름
    void detail();   // 상세 조회 흐름
    void update();   // 수정 흐름
    void remove();   // 삭제 흐름
private:
    ItemRepository repo_;  // 저장소
    ItemView       view_;  // 화면
};
```

Controller는 **View와 Repository를 연결하는 접착제**입니다. 직접 `cout`을 쓰지 않고 View에 위임합니다.

```cpp
// 등록 흐름 예시
void ItemController::create() {
    Item item = view_.inputItem();          // (1) View 에서 입력 수집
    if (item.name.empty()) {
        view_.showError("이름은 필수");     // (2) 유효성 검사 → View 에 오류 표시
        return;
    }
    Item saved = repo_.save(item);          // (3) Repository 에 저장 요청
    view_.showSuccess("등록 완료: " + saved.id); // (4) 결과를 View 에 전달
}
```

---

### 3-5. main.cpp — 진입점

```cpp
int main() {
    SetConsoleOutputCP(CP_UTF8);   // Windows 콘솔 UTF-8 설정 (한글 출력)
    SetConsoleCP(CP_UTF8);

    ItemController controller;
    ItemView       view;

    // 메뉴 선택 → 함수 매핑 테이블
    const std::unordered_map<std::string, std::function<void()>> actions = {
        {"1", [&]{ controller.create();  }},
        {"2", [&]{ controller.listAll(); }},
        {"3", [&]{ controller.detail();  }},
        {"4", [&]{ controller.update();  }},
        {"5", [&]{ controller.remove();  }},
    };

    while (true) {
        std::string choice = view.showMenu();   // View 에서 선택값 수신
        if (choice == "0") break;
        auto it = actions.find(choice);
        if (it != actions.end()) it->second();  // 해당 Controller 함수 호출
        else view.showError("올바른 번호를 입력하세요.");
    }
}
```

- `unordered_map<string, function<void()>>` — if-else 분기 대신 **디스패치 테이블** 방식으로 메뉴를 처리합니다. 메뉴가 늘어도 테이블에 한 줄 추가만 하면 됩니다.

---

## 4. 코드 동작 흐름

### 전체 시퀀스

```
main()
  │
  ├─ SetConsoleOutputCP(CP_UTF8)   ← 한글 깨짐 방지
  │
  └─ while(true)
       │
       ├─ view.showMenu()           ← [View] 메뉴 출력 후 입력 대기
       │     반환: "1"~"5" 또는 "0"
       │
       ├─ choice == "0" → break
       │
       └─ actions[choice]()         ← Controller 의 해당 함수 호출
```

---

### 시나리오 A — 아이템 등록 (`1` 선택)

```
사용자: "1" 입력
  │
  ▼
main() → controller.create() 호출
  │
  ▼
[Controller] create()
  ├─ view_.inputItem()              ← [View] 이름/설명/수량 입력 요청
  │     반환: Item{name="웨이퍼", quantity=100, ...}
  │
  ├─ item.name.empty() 검사
  │     비어있으면 → view_.showError() 후 return
  │
  ├─ repo_.save(item)               ← [Repository] 저장 요청
  │     │
  │     ├─ load()                   ← data/items.json 읽기
  │     ├─ nextId()                 ← "ITEM-001" 채번
  │     ├─ items.push_back(item)
  │     ├─ persist()                ← data/items.json 쓰기
  │     └─ 반환: Item{id="ITEM-001", ...}
  │
  └─ view_.showSuccess("등록 완료") ← [View] 결과 출력
```

---

### 시나리오 B — 아이템 삭제 (`5` 선택)

```
사용자: "5" 입력
  │
  ▼
controller.remove() 호출
  │
  ├─ view_.inputId("삭제")          ← [View] "삭제할 ID" 입력 요청
  │     반환: "ITEM-002"
  │
  ├─ repo_.findById("ITEM-002")     ← [Repository] 존재 여부 확인
  │     없으면 → view_.showError() 후 return
  │
  ├─ view_.showItemDetail(*item)    ← [View] 삭제 대상 정보 표시
  │
  ├─ view_.confirm("정말 삭제?")    ← [View] Y/N 확인
  │     N → view_.showSuccess("취소") 후 return
  │
  ├─ repo_.remove("ITEM-002")       ← [Repository] 삭제 실행
  │     load() → erase() → persist()
  │
  └─ view_.showSuccess("삭제 완료") ← [View] 결과 출력
```

---

### 데이터 흐름 요약

```
[사용자 입력]
      │ string / int
      ▼
   ItemView         ← cout/cin 전담 (UI 경계)
      │ Item struct
      ▼
 ItemController     ← 흐름 연결 (로직 없음)
      │ Item struct
      ▼
 ItemRepository     ← 파일 I/O 전담
      │ nlohmann::json
      ▼
  data/items.json   ← 영속성 저장소
```

---

## 5. 프로젝트 구조

```
ConsoleMVC-JOYUSIK-21044893/
├── ConsoleMVC/                     ← Visual Studio 프로젝트
│   ├── ConsoleMVC.vcxproj          ← VS 프로젝트 파일 (빌드 설정)
│   ├── ConsoleMVC.vcxproj.filters  ← VS 솔루션 탐색기 폴더 구조
│   ├── main.cpp                    ← 진입점, 메뉴 디스패치
│   ├── model/
│   │   ├── Item.h                  ← 데이터 구조 정의
│   │   └── Item.cpp                ← toJson / fromJson 구현
│   ├── repository/
│   │   ├── ItemRepository.h
│   │   └── ItemRepository.cpp      ← JSON 파일 CRUD
│   ├── view/
│   │   ├── ItemView.h
│   │   └── ItemView.cpp            ← 모든 cout/cin
│   ├── controller/
│   │   ├── ItemController.h
│   │   └── ItemController.cpp      ← 흐름 제어
│   ├── third_party/
│   │   └── json.hpp                ← nlohmann/json 3.11.3 (헤더 온리)
│   └── data/                       ← 런타임 생성 (git 제외)
│       └── items.json
├── CMakeLists.txt                  ← CMake 빌드 (선택)
├── .gitignore
└── README.md
```

---

## 6. 빌드 및 실행

### Visual Studio에서 열기

1. Visual Studio 실행
2. `파일 > 열기 > 프로젝트/솔루션`
3. `ConsoleMVC\ConsoleMVC.slnx` (또는 `ConsoleMVC.vcxproj`) 선택
4. 상단 구성: `Debug | x64` 선택
5. **F7** 빌드 → **F5** 실행

> `data/items.json`은 최초 실행 시 자동으로 생성됩니다.

### 실행 화면

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

---

## 환경 정보

| 항목 | 내용 |
|---|---|
| 언어 | C++20 |
| IDE | Visual Studio 2025 Preview (v18) |
| Toolset | MSVC v145 |
| JSON | nlohmann/json 3.11.3 (헤더 온리) |
| 인코딩 | UTF-8 (`/utf-8` 컴파일 옵션) |
