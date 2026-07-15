# ConsoleMVC-JOYUSIK-21044893

MVC 패턴 구조 검증을 위한 PoC 프로젝트입니다.

## 목적

반도체 시료 생산주문관리 시스템(미션2) 구현에 앞서,
Model / View / Controller 레이어 분리가 올바르게 동작하는지 검증합니다.

## 구조

```
ConsoleMVC/
├── model/item.py              # 순수 데이터 클래스 (비즈니스 로직 없음)
├── repository/item_repo.py    # JSON 파일 CRUD
├── controller/item_controller.py  # 흐름 제어, Repository 호출
├── view/item_view.py          # 콘솔 출력 + 사용자 입력
└── main.py                    # 진입점, 메뉴 루프
```

## 레이어 역할

| 레이어 | 역할 | 금지사항 |
|---|---|---|
| Model | 순수 데이터 구조 | 비즈니스 로직, I/O |
| Repository | JSON 파일 CRUD | 비즈니스 판단 |
| Controller | 흐름 제어 | 직접 출력, 비즈니스 로직 |
| View | 화면 출력 + 입력 | 비즈니스 로직, 데이터 저장 |

## 실행

```bash
python main.py
```

## 요구사항

- Python 3.10+
- 외부 라이브러리 없음
