# 廣義代數函數求和公式

## 專案概述

此為「廣義代數函數求和公式」之附屬專案，為參加113學年度臺中市中小學科學展覽會建立。

本專案探討代數函數的求和公式，並研究GPHA和SPHA的性質。

## 專案結構與實作細節

### Python部分

Python程式重視便利性。需要Python 3.8以上版本和NumPy。

**※** 執行Python腳本前，需修改每個腳本第一行的檔案路徑，使其符合本機中`B.txt`的位置。

- `B.txt`：白努利數庫（至B_300）
- `SPHAmod.py`：含SPHA基本運算之模組
- `Summation_R.py`, `Summation_C.py`：
  - 分別計算實數和複數次方的求和
- `Summation_SPHA.py`：
  - 計算SPHA次方的求和
  - 導入`SPHAmod.py`
- `Summation_mod_SPHA.py`：
  - 獨立的SPHA求和計算器

### C++部分

C++適合大規模計算。需要C++17相容編譯器。

- `B.cpp`：
  - Bernoulli數計算器
- `B.txt`：白努利數庫（至B_1000）
- `SPHAmod.cpp` / `SPHAmod.h`：
  - 含SPHA基本運算之模組
- `Summation_SPHA.cpp` / `Summation_SPHA.exe`：
  - SPHA求和計算器
  - 導入`SPHAmod.h`
- `Summation_mod_SPHA.cpp` / `Summation_mod_SPHA.exe`：
  - 獨立的SPHA求和計算器

### 網頁部分

網頁提供SPHA計算的互動介面。

[專案網站](https://william0506.github.io/Summation/HTML/Summation.html)

- `SPHAmod.mjs`：
  - 含SPHA基本運算之模組
  - 含SPHA算術和工具函數
- `Summation.html` / `Summation.mjs`：
  - 網頁介面和主要程式碼，用於SPHA求和計算
  - **預設值**：
    - 若field num為空：對n>=0，預設為(0,0,0)；對n<0，預設為(1,0,0)
    - 對Power輸入：實部後缺少的部分將填充0
    - Power輸入的多餘項將自動截斷
    - 精度預設為10；若超過最大限制(300)，自動設為300

## 技術需求

### 開發環境
- Python：Version 3.8或更高版本並安裝NumPy
- C++：C++17相容編譯器
- 網頁：支援ES6 module的瀏覽器(Chrome 61+, Firefox 60+, Safari 11+)

### 補充說明
1. Web版需啟用CORS
2. ES6 modules必須由web server提供

## 使用說明

### 命令列工具（Python和C++）
1. 設置所需的開發環境
2. 安裝必要的依賴項
3. Python腳本：更新腳本標頭中的檔案路徑
4. 執行腳本/程式並提供適當參數

**※**：
由於負數的實數次方可能得到複數，當求和的upperbound為負數時，field num不能設為(0,0,0)。在此情況下，程式會自動將其設為(1,0,0)。

### 網頁介面
1. 使用現代瀏覽器存取
2. 在指定欄位輸入參數
3. 點擊"Calculate"執行計算
