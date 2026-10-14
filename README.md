# chatByAI

**平台工具：** Qt 5.14.2 + Qt Creator 4.11.1、qmake 构建、MinGW 7.3（32/64 位双工具链）、Qt Designer 可视化布局、OpenSSL 1.1 动态库、Windows 桌面端

**项目内容：**

1. 基于 Qt Network 模块的 QNetworkAccessManager / QNetworkRequest / QNetworkReply，以**全异步非阻塞**方式对接 DeepSeek 开放 API（OpenAI 兼容的 `/chat/completions` 接口），主线程全程不阻塞，UI 保持可响应
2. 使用 QJsonObject / QJsonArray / QJsonDocument 完成请求体与响应体的 **JSON 序列化与反序列化**，按 role/content 组装 system + user 多角色对话消息
3. 基于 **SSE（Server-Sent Events）流式协议**实现"打字机"式增量输出：逐行读取 `data:` 分片、过滤结束标记、解析 `choices[0].delta.content` 并实时追加渲染，显著降低首字延迟
4. 基于 **Qt 信号-槽机制 + Lambda 表达式**实现异步回调，将"发送请求"与"处理响应/结束清理"解耦，并通过 `deleteLater()` 管理 Reply 生命周期，规避内存泄漏与野指针
5. 集成 **OpenSSL（libssl / libcrypto）动态库**，通过 QSslSocket 校验 TLS 能力，保障 HTTPS 加密通信握手成功
6. 使用 **Qt Designer（.ui）**完成输出区 / 输入区 / 按钮的界面与逻辑分离，qmake 工程支持 32/64 位 MinGW 工具链编译
7. 通过 `QSslSocket::supportsSsl()` / `sslLibraryVersionString()` 等接口完成 **TLS 环境自检**，定位并解决 Windows 平台 HTTPS 依赖问题
8. 使用 `QJsonParseError` 对流式分片做**协议级容错**，非 JSON 帧静默跳过，保证长连接流不被异常帧中断

#### 其它：待升级处
- 引入**多轮对话上下文**（维护 history 消息数组）与会话本地持久化；
- 增加 **Markdown 渲染与代码块高亮**，提升回答可读性；
- 抽离独立 `ApiClient` 网络层，增加**超时、重试、流式中断**与 HTTP 错误码（401/429/5xx）分类提示；
- **API Key 安全化**：迁移至环境变量 / QSettings / 系统密钥库，避免明文硬编码；
- 使用 **QThread / QtConcurrent** 支持多会话并发请求，支持多模型切换。
