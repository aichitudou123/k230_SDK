import os
from openai import OpenAI

# 下面的 API 是从淘宝上买的 OpenAI 国内中转 API
# 使用下面的 API Key 的时候务必断开梯子
# 淘宝上搜索 API Key , 可以搜出一大堆的 中转 或 直联 的 OpenAI 接口

# 设置 OPENAI_API_KEY 环境变量
os.environ["OPENAI_API_KEY"] = "sk-mQ6HoW8EprruhS7qoXsLCgkODMxGyxYBgy6hlLL7Uq3VJNuU"
# 设置 OPENAI_BASE_URL 环境变量
os.environ["OPENAI_BASE_URL"] = "https://api.chatanywhere.tech"


def openai_chat():
    client = OpenAI(
        # 下面两个参数的默认值来自环境变量，可以不加
        api_key=os.environ.get("OPENAI_API_KEY"),
        base_url=os.environ.get("OPENAI_BASE_URL"),
    )
    # 创建一个聊天完成请求
    completion = client.chat.completions.create(
        # 指定模型为"gpt-3.5-turbo"
        model="gpt-3.5-turbo",
        # 定义对话消息列表
        messages=[
            # 系统角色的消息，用于设置对话的起始状态
            {"role": "system", "content": "You are a helpful assistant."},
            # 用户角色的消息，用于指示用户的输入
            {"role": "user", "content": "你好"}
        ]
    )
    print(completion)  # 响应
    print(completion.choices[0].message)  # 回答


if __name__ == '__main__':
    openai_chat()

