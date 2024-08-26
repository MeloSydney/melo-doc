
//TODO - refs
https://soft.xiaoshujiang.com/docs/grammar/feature/mermaid/#e5ba8fe58897e59bbe_26

//TODO - drawio Mermaid UML

sequenceDiagram
        participant A as 用户
        participant B as Harmony设备
        participant C as 云服务器
        participant D as 云数据库
        participant E as 嵌入式上层应用
        participant F as 开发板
        participant G as SQLite

        activate A #grey
        note over A: 用户查看操作
        A->>B: 查询事件
        deactivate A
        activate B #lightblue
        B->>C: 请求数据
        deactivate B
        activate C #orange
        C->>D: 查询
        deactivate C
        activate D #blue
        D-->>C: 返回数据
        deactivate D
        activate C #orange
        C-->>B: 返回数据
        deactivate C
        activate B #lightblue
        B-->>A: 显示内容
        deactivate B

        activate A #grey
        note over A: 用户更改操作
        A->>B: 更新事件
        deactivate A
        activate B #lightblue
        B->>C: 请求更新
        deactivate B
        activate C #orange
        C->>F: 请求更新
        deactivate C
        activate F #lightgreen
        F->>G: 请求更新
        deactivate F
        activate G #green
        G-->>F: 返回结果
        deactivate G
        activate F #lightgreen
        F->>C: 发送修改请求
        deactivate F
        activate C #orange
        C->>D: 更新数据
        deactivate C
        activate D #blue
        D-->>C: 返回结果
        deactivate D
        activate C #orange
        C-->>B: 返回结果
        deactivate C
        activate B #lightblue
        B-->>A: 返回结果
        deactivate B
        activate A #grey
        A->>A: 提示结果
        note over A: 操作成功
        deactivate A

//TODO - npclk

sequenceDiagram
        participant A as clkA
        participant B as clkB
        participant C as clkC

        activate A
        A->>A: self cannot round-rate
        A->>B: ask parent round-rate
        activate B
        B->>B: self cannot round-rate
        B->>C: ask parent round-rate
        activate C
        C->>C: (*round_rate)
        Note right of C: j5没有determine_rate
        C->>C: update p_index
        Note right of C: round_rate不会改变parent
        C->>C: calc parent new-rate if best_parent_rate changed
        Note right of C: 没有设置CLK_SET_RATE_PARENT
        C->>C: clk_calc_subtree with new_rate
        C->>B: return
        deactivate C
        B->>B: clk_calc_subtree with new_rate
        B->>A: return
        deactivate B
        A->>A: clk_calc_subtree with new_rate
        deactivate A


sequenceDiagram
        participant A as clkA
        participant B as clkA-child
        participant C as clkA-new_child

        activate A
        A->>A: get correct parent and best_parent_rate
        A->>A: (*set_parent)
        A->>A: (*set_rate)
        A->>A: core->rate = clk_recalc(core, best_parent_rate)
        opt if rate changed
                A->>A: notify POST_RATE_CHANGE
        end
        opt if set CLK_RECALC_NEW_RATES
                A->>A: clk_calc_new_rates
        end
        loop Every child
                A->>B: clk_change_rate(child);
                activate B
                B->>B: get correct parent and best_parent_rate
                B->>B: (*set_parent)
                B->>B: (*set_rate)
                B->>B: core->rate = clk_recalc(core, best_parent_rate)
                opt if rate changed
                        B->>B: notify POST_RATE_CHANGE
                end
                opt if set CLK_RECALC_NEW_RATES
                        B->>B: clk_calc_new_rates
                end
                B->>A: return
        end
        deactivate B
        loop Every new_child
                A->>C: clk_change_rate(core->new_child);
                activate C
                C->>C: get correct parent and best_parent_rate
                C->>C: (*set_parent)
                C->>C: (*set_rate)
                C->>C: core->rate = clk_recalc(core, best_parent_rate)
                opt if rate changed
                        C->>C: notify POST_RATE_CHANGE
                end
                opt if set CLK_RECALC_NEW_RATES
                        C->>C: clk_calc_new_rates
                end
                C->>A: return
        end
        deactivate C



