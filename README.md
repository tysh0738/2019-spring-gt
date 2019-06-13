# Graph-Theory-Project-Template
Project template of course - Graph Theory (EE6622E) in National Cheng Kung University.

## 學號: E24044046

## 解題邏輯
此程式為解決無向圖之中國郵差問題。(此圖為simple graph，且權重皆相同)
測資的部分，因使用topo讀檔產生的為有向圖，因此測資使用雙向edge表示無向圖，並且每條edges的capacity和flow value皆設為1。
-流程:
1. 產生無向圖
2. 計算各點的degree，並找出odd degree的點
3. 對全部odd degree的點做maximum matching
4. 找到Euler circuit，即為所求

## 程式碼實作
1. 使用interpret函式將topo.txt檔建出無向圖
2. 計算各點的degree存在一維的動態陣列(degree)裡，並記錄degree為奇數的點於另一個vector(odd_vertex)
3. 若是有degree為奇數的點，則使用助教提供的Path structure找到每一種matching的path，再計算分別最短的path長度，並將path長度和經過的edges記錄在兩個vector中(path_length, path_vertex)。最後比較出全部最短path長度的matching，即為maximum matching。將maximum matching需要經過的eges在原圖加上相對應的edges，亦使用雙向edge，並將原本圖中相對應edges的capacity設為3，作為後面演算法的指標。
若是degree全為偶數，則跳過這一個步驟。
-此外，此步驟的maximum matching目前只完成2或4個vertices的matching。
4. 使用新產生的圖，從第一個vertex開始，檢查是否有與其他vertices connected，若是connected並且capacity=3(代表需要重複走的路)，則下一步就走這條edge；若是capacity!=3，則逐次搜尋每個edges的connected狀況，若是其他edge都沒有與目前的vertex connected，則此條edge就為下一步。其中，走過的edge要雙向disconnect，並且將走過的vertices存在一個vector(circuit)中。執行直到全部edges都走完，並且走回原點。
輸出:circuit的長度為新產生圖的edge個數除以2，因為edge是創建雙向的(兩條)。並將存在vector(circuit)的edges依序輸出。

## 執行結果
### 1. topo1.txt: 4 odd degree's vertices
-graph:

![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/graph1.PNG)

-result:

![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/topo1.PNG)

### 2. topo2.txt: 0 odd degree's vertices
-graph:

![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/graph2.PNG)

-result:

![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/topo2.PNG)

### 3. topo3.txt: 2 odd degree's vertices
-graph:

![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/graph3.PNG)

-result:
![](https://github.com/tysh0738/2019-spring-gt/blob/master/img/topo3.PNG)
