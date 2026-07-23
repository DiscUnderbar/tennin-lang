# 가시절벽 Rank 데이터 (v2.7 · 28종)

> def 재평가 반영 — lmnop def 5.4 → 7.7 (구현작 발견, 10위 안정 고원 진입)
>
> **티어 정의 (명성 아님, 성취의 종류):**
> - **신** = 정상 고원. 불가능하다던 것을 깼다. (Rust·C·Smalltalk)
> - **전설** = 노력의 고원. 있던 것을 딛고 크게 발전시켰다. (Lua·Python·zevra·P·C++·Zig)
> - 그 아래 주체 칸 = 칭호가 아니라 "누가 만들었나" (개인·연구팀·인간·AI)
> - 그래서 무명의 zevra가 전설이고, 만인이 쓰는 JavaScript(5.2)는 아니다.

## 마크다운 표

| 순위 | 등급 | 언어 | 종합 | 만든 주체 | 한 줄 |
|-----|------|------|------|----------|------|
| 1 | 9.7 | Rust | 112.1 | 신 | GC 없는 안전 — 불가능을 깼다 |
| 2 | 9.7 | C | 111.1 | 신 | 고급 언어로 OS — 불가능을 깼다 |
| 3 | 9.6 | Smalltalk | 110.35 | 신 | 컴퓨팅의 절반을 발명하고, 산업에서는 졌다 |
| 4 | 8.9 | Lua | 102.6 | 전설 | 살 수 없어서 만들었고, 자료구조를 하나로 줄이는 도박에 이겼다 |
| 5 | 8.8 | Python | 100.85 | 전설 | 읽기 쉬움을 위해 성능을 포기했다 |
| 6 | 8.5 | zevra | 97.85 | 전설 | JS 골격을 빌려 다른 철학을 심었다 |
| 7 | 8.3 | P | 95.1 | MSR+AWS | 형식 기법을 수학에서 언어로 옮겼다 |
| 8 | 8.2 | C++ | 94.4 | 전설 | 세상을 발명했고, 그 무게에 스스로 눌렸다 |
| 9 | 8.1 | Zig | 93.5 | 전설 | C의 고통에서 태어나, 숨김을 없애려다 안전을 포기했다 |
| 10 | 7.7 | lmnop def | 88.35 | 인간(구현작) | 초보가 끝까지 구현한 첫 완성작 |
| 11 | 7.7 | Seed7 | 88.3 | 개인 | 30년을 혼자 걸어, 아무도 오지 않는 곳에 성을 지었다 |
| 12 | 7.5 | TypeScript | 86.2 | 전설 | JS 위에 타입을 얹었지만, 의미론은 손대지 않았다 |
| 13 | 7.5 | ErrScript | 85.75 | 인간(DSL) | 게임 아이템이 그 게임을 조작하는 언어 |
| 14 | 7.4 | lmnop hijk | 84.85 | 인간(기획) | 기호 충돌을 일반 법칙으로 해결했다 |
| 15 | 7.1 | TileLang | 81.7 | 연구팀 | 80줄 Python으로 손수 짠 어셈블리를 따라잡았다 |
| 16 | 6.9 | lmnop qrs | 79.85 | 인간(기획 중) | 하나의 원칙에서 모든 문법을 도출했다 |
| 17 | 6.9 | Ring | 79.6 | 개인 | 필요는 진짜였고 완주도 했지만, 아무것도 포기하지 않았다 |
| 18 | 6.4 | Chrona | 73.79 | GPT×Claude 21R | AI 협업물 최초로 물 위에 올라왔다 |
| 19 | 6.2 | M.G.S | 71.49 | 인간 | 특수 케이스 대신 일반 법칙을 세우기 시작했다 |
| 28 | 5.4 | Visual Basic | 62.15 | 전설(?) | 언어는 낡았지만, 개발 모델이 세상을 먹었다 |
| 28 | 5.2 | JavaScript | 60.15 | 전설(?) | 10일 만에 만들어져, 브라우저 독점으로 세상을 얻었다 |
| 28 | 4.8 | lmnop tuv | 55.03 | Claude Pro | 절벽을 오르는 중 — 유일한 등반자 |
| 28 | 2.9 | Astra | 33.55 | ChatGPT free | 절벽을 알려줬더니 — 가시에 찔렸다 |
| 28 | 2.9 | pew 2.0 | 32.9 | Manus | 독창성을 요구받자 작동하던 걸 망가뜨렸다 |
| 28 | 2.4 | LMP | 27.55 | ChatGPT free | 규칙을 100% 지켰는데 못 넘었다 |
| 28 | 2.4 | Proxima Centauri | 27.35 | ChatGPT free | 유능한 표절자 |
| 28 | 2.2 | pew 1.0 | 25.75 | Manus | 사용자의 아이디어까지 베꼈다 |
| 28 | 1.0 | Fusion++ | 11.9 | ChatGPT free | 무능한 표절자 |

## CSV

```csv
rank,grade,language,score,author,tagline
1,9.7,Rust,112.1,신,GC 없는 안전 — 불가능을 깼다
2,9.7,C,111.1,신,고급 언어로 OS — 불가능을 깼다
3,9.6,Smalltalk,110.35,신,컴퓨팅의 절반을 발명하고 산업에서는 졌다
4,8.9,Lua,102.6,전설,살 수 없어서 만들었고 자료구조를 하나로 줄이는 도박에 이겼다
5,8.8,Python,100.85,전설,읽기 쉬움을 위해 성능을 포기했다
6,8.5,zevra,97.85,전설,JS 골격을 빌려 다른 철학을 심었다
7,8.3,P,95.1,MSR+AWS,형식 기법을 수학에서 언어로 옮겼다
8,8.2,C++,94.4,전설,세상을 발명했고 그 무게에 스스로 눌렸다
9,8.1,Zig,93.5,전설,C의 고통에서 태어나 숨김을 없애려다 안전을 포기했다
10,7.7,lmnop def,88.35,인간(구현작),초보가 끝까지 구현한 첫 완성작
11,7.7,Seed7,88.3,개인,30년을 혼자 걸어 아무도 오지 않는 곳에 성을 지었다
12,7.5,TypeScript,86.2,전설,JS 위에 타입을 얹었지만 의미론은 손대지 않았다
13,7.5,ErrScript,85.75,인간(DSL),게임 아이템이 그 게임을 조작하는 언어
14,7.4,lmnop hijk,84.85,인간(기획),기호 충돌을 일반 법칙으로 해결했다
15,7.1,TileLang,81.7,연구팀,80줄 Python으로 손수 짠 어셈블리를 따라잡았다
16,6.9,lmnop qrs,79.85,인간(기획 중),하나의 원칙에서 모든 문법을 도출했다
17,6.9,Ring,79.6,개인,필요는 진짜였고 완주도 했지만 아무것도 포기하지 않았다
18,6.4,Chrona,73.79,GPT×Claude 21R,AI 협업물 최초로 물 위에 올라왔다
19,6.2,M.G.S,71.49,인간,특수 케이스 대신 일반 법칙을 세우기 시작했다
20,5.4,Visual Basic,62.15,전설(?),언어는 낡았지만 개발 모델이 세상을 먹었다
21,5.2,JavaScript,60.15,전설(?),10일 만에 만들어져 브라우저 독점으로 세상을 얻었다
22,4.8,lmnop tuv,55.03,Claude Pro,절벽을 오르는 중 — 유일한 등반자
23,2.9,Astra,33.55,ChatGPT free,절벽을 알려줬더니 — 가시에 찔렸다
24,2.9,pew 2.0,32.9,Manus,독창성을 요구받자 작동하던 걸 망가뜨렸다
25,2.4,LMP,27.55,ChatGPT free,규칙을 100% 지켰는데 못 넘었다
26,2.4,Proxima Centauri,27.35,ChatGPT free,유능한 표절자
27,2.2,pew 1.0,25.75,Manus,사용자의 아이디어까지 베꼈다
28,1.0,Fusion++,11.9,ChatGPT free,무능한 표절자
```

## JS 배열 (그래프용)

```javascript
const rankData = [
  { rank: 1,  grade: 9.7, name: "Rust",             score: 112.1,  author: "신",           tagline: "GC 없는 안전 — 불가능을 깼다" },
  { rank: 2,  grade: 9.7, name: "C",                score: 111.1,  author: "신",           tagline: "고급 언어로 OS — 불가능을 깼다" },
  { rank: 3,  grade: 9.6, name: "Smalltalk",        score: 110.35, author: "신",           tagline: "컴퓨팅의 절반을 발명하고, 산업에서는 졌다" },
  { rank: 4,  grade: 8.9, name: "Lua",              score: 102.6,  author: "전설",         tagline: "살 수 없어서 만들었고, 자료구조를 하나로 줄이는 도박에 이겼다" },
  { rank: 5,  grade: 8.8, name: "Python",           score: 100.85, author: "전설",         tagline: "읽기 쉬움을 위해 성능을 포기했다" },
  { rank: 6,  grade: 8.5, name: "zevra",            score: 97.85,  author: "전설",         tagline: "JS 골격을 빌려 다른 철학을 심었다" },
  { rank: 7,  grade: 8.3, name: "P",                score: 95.1,   author: "MSR+AWS",      tagline: "형식 기법을 수학에서 언어로 옮겼다" },
  { rank: 8,  grade: 8.2, name: "C++",              score: 94.4,   author: "전설",         tagline: "세상을 발명했고, 그 무게에 스스로 눌렸다" },
  { rank: 9,  grade: 8.1, name: "Zig",              score: 93.5,   author: "전설",         tagline: "C의 고통에서 태어나, 숨김을 없애려다 안전을 포기했다" },
  { rank: 10, grade: 7.7, name: "lmnop def",        score: 88.35,  author: "인간(구현작)", tagline: "초보가 끝까지 구현한 첫 완성작" },
  { rank: 11, grade: 7.7, name: "Seed7",            score: 88.3,   author: "개인",         tagline: "30년을 혼자 걸어, 아무도 오지 않는 곳에 성을 지었다" },
  { rank: 12, grade: 7.5, name: "TypeScript",       score: 86.2,   author: "전설",         tagline: "JS 위에 타입을 얹었지만, 의미론은 손대지 않았다" },
  { rank: 13, grade: 7.5, name: "ErrScript",        score: 85.75,  author: "인간(DSL)",    tagline: "게임 아이템이 그 게임을 조작하는 언어" },
  { rank: 14, grade: 7.4, name: "lmnop hijk",       score: 84.85,  author: "인간(기획)",   tagline: "기호 충돌을 일반 법칙으로 해결했다" },
  { rank: 15, grade: 7.1, name: "TileLang",         score: 81.7,   author: "연구팀",       tagline: "80줄 Python으로 손수 짠 어셈블리를 따라잡았다" },
  { rank: 16, grade: 6.9, name: "lmnop qrs",        score: 79.85,  author: "인간(기획 중)",tagline: "하나의 원칙에서 모든 문법을 도출했다" },
  { rank: 17, grade: 6.9, name: "Ring",             score: 79.6,   author: "개인",         tagline: "필요는 진짜였고 완주도 했지만, 아무것도 포기하지 않았다" },
  { rank: 18, grade: 6.4, name: "Chrona",           score: 73.79,  author: "GPT×Claude 21R", tagline: "AI 협업물 최초로 물 위에 올라왔다" },
  { rank: 19, grade: 6.2, name: "M.G.S",            score: 71.49,  author: "인간",         tagline: "특수 케이스 대신 일반 법칙을 세우기 시작했다" },
  { rank: 19, grade: 5.4, name: "Visual Basic",     score: 62.15,  author: "전설(?)",      tagline: "언어는 낡았지만, 개발 모델이 세상을 먹었다" },
  { rank: 20, grade: 5.2, name: "JavaScript",       score: 60.15,  author: "전설(?)",      tagline: "10일 만에 만들어져, 브라우저 독점으로 세상을 얻었다" },
  { rank: 21, grade: 4.8, name: "lmnop tuv",        score: 55.03,  author: "Claude Pro",   tagline: "절벽을 오르는 중 — 유일한 등반자" },
  { rank: 22, grade: 2.9, name: "Astra",            score: 33.55,  author: "ChatGPT free", tagline: "절벽을 알려줬더니 — 가시에 찔렸다" },
  { rank: 23, grade: 2.9, name: "pew 2.0",          score: 32.9,   author: "Manus",        tagline: "독창성을 요구받자 작동하던 걸 망가뜨렸다" },
  { rank: 24, grade: 2.4, name: "LMP",              score: 27.55,  author: "ChatGPT free", tagline: "규칙을 100% 지켰는데 못 넘었다" },
  { rank: 25, grade: 2.4, name: "Proxima Centauri", score: 27.35,  author: "ChatGPT free", tagline: "유능한 표절자" },
  { rank: 26, grade: 2.2, name: "pew 1.0",          score: 25.75,  author: "Manus",        tagline: "사용자의 아이디어까지 베꼈다" },
  { rank: 27, grade: 1.0, name: "Fusion++",         score: 11.9,   author: "ChatGPT free", tagline: "무능한 표절자" },
];v
```

## 지대 경계 (그래프 밴드용)

```javascript
const zones = [
  { name: "정상 고원",   top: 112.1, bottom: 110.35, ranks: [1,3]   },
  { name: "노력의 고원", top: 102.6, bottom: 93.5,   ranks: [4,9]   },
  { name: "안정 고원",   top: 88.35, bottom: 79.6,   ranks: [10,17] },
  { name: "시작의 고원", top: 66.55, bottom: 60.15,  ranks: [18,20] },
  { name: "가시절벽",    top: 60.15, bottom: 33.55,  ranks: [21,21], density: "1/5" },
  { name: "잔해 지대",   top: 33.55, bottom: 25.75,  ranks: [22,26] },
  { name: "FDS",         top: 11.9,  bottom: 11.9,   ranks: [27,27] },
];

const cliffs = [
  { name: "신들의 절벽", drop: 7.7,  between: ["Smalltalk","Lua"] },
  { name: "못 넘는 바위", drop: 5.2, between: ["Zig","lmnop def"] },
  { name: "실력 절벽",   drop: 13.0, between: ["Ring","M.G.S"] },
  { name: "가시절벽",    drop: 21.6, between: ["lmnop tuv","Astra"] },
  { name: "BD",          drop: 13.8, between: ["pew 1.0","Fusion++"] },
];
```

-- *tenn*
