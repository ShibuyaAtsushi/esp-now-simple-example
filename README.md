# ESP-NOW Simple Example / ESP-NOW 簡単な例

## Overview / 概要

A beginner-friendly example of using ESP-NOW. This project focuses on getting basic communication working without complex features.

ESP-NOWを使う初心者向けの簡単な例です。複雑な機能は省いて、まずは基本的な通信を動かすことに焦点を当てています。

## Features / 特徴

### Device Roles / デバイスの役割
- Transmitter / 送信側: m5AtomS3Lite
- Receiver / 受信側: m5core2

### Key ESP-NOW Settings / ESP-NOWの主要設定
To use ESP-NOW, the following settings are necessary:

ESP-NOWを使用するには、以下の設定が必要です：

1. Configure the WiFi channel to be used
   使用するWiFiチャンネルの設定

2. Add the MAC address of the intended receiver on the transmitter side
   送信側で、送信先（受信側）のMACアドレスを追加

This sample demonstrates how to implement these basic settings and achieve simple communication using ESP-NOW.

このサンプルでは、上記の基本的な設定方法と、ESP-NOWを使用したシンプルな通信の実装方法を示しています。
