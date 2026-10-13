#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnSend_clicked()
{
    // 获取文本框内容
    QString text = ui->txtEInput->toPlainText();

    ui->txtEInput->clear();

    ui->txtEOutput->insertPlainText("-->>");
    ui->txtEOutput->insertPlainText(text);
    ui->txtEOutput->insertPlainText("\n\n\n");

    QNetworkRequest req;
    req.setUrl(QUrl("https://api.deepseek.com/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    req.setRawHeader("Authorization", "Bearer 替换成API-key-info");

    QJsonArray msg;

    QJsonObject systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] = "You are a programer assistant";


    QJsonObject userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = text;

    msg.append(systemMsg);
    msg.append(userMsg);

    QJsonObject reqBody;
    reqBody["messages"] = msg;
    reqBody["model"] = "deepseek-v4-pro";
    reqBody["max_tokens"] = 4096;
    reqBody["stream"] = true;
    reqBody["temperature"] = 1;

    // 发送请求
    QNetworkReply *reply = manager->post(req, QJsonDocument(reqBody).toJson());

    // 处理数据
    connect(reply, &QNetworkReply::readyRead, this, [=](){
        while(reply->canReadLine())
        {
            QString line = reply->readLine().trimmed();

            if(line.startsWith("data: "))   // 以data: 开头的数据
            {
                line.remove(0, 6);

                QJsonParseError err;

                QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &err);

                if(err.error == QJsonParseError::NoError)
                {
                    QString content = doc.object()["choices"].toArray().first()
                            .toObject()["delta"].toObject()["content"].toString();

                    if(!content.isEmpty())
                    {
                        ui->txtEOutput->moveCursor(QTextCursor::End);
                        ui->txtEOutput->insertPlainText(content);
                    }
                }
            }
        }
    });

    connect(reply, &QNetworkReply::finished, this, [=](){
        ui->txtEOutput->moveCursor(QTextCursor::End);
        ui->txtEOutput->insertPlainText("\n\n\n");

        reply->deleteLater();
    });
}















