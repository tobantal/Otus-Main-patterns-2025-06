#!/bin/bash

# Production ключи
openssl genrsa -out production/private_key.pem 2048
openssl rsa -in production/private_key.pem -pubout -out production/public_key.pem

# Test ключи
openssl genrsa -out testing/private_key.pem 2048
openssl rsa -in testing/private_key.pem -pubout -out testing/public_key.pem
