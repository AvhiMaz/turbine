#!/bin/bash

RPC="https://mainnet.helius-rpc.com/?api-key=impoooor:sob:sob"
OUTPUT="data/transactions.json"
TARGET=10000

echo "fetching latest slot..."
SLOT=$(curl -s -X POST "$RPC" \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","id":1,"method":"getSlot"}' | jq '.result')

echo "latest slot: $SLOT"

echo "[" > "$OUTPUT"
COUNT=0
FIRST=1

while [ $COUNT -lt $TARGET ]; do
  echo "fetching block $SLOT... (collected: $COUNT)"

  BLOCK=$(curl -s -X POST "$RPC" \
    -H "Content-Type: application/json" \
    -d "{
      \"jsonrpc\": \"2.0\",
      \"id\": 1,
      \"method\": \"getBlock\",
      \"params\": [
        $SLOT,
        {
          \"encoding\": \"base64\",
          \"transactionDetails\": \"full\",
          \"maxSupportedTransactionVersion\": 0
        }
      ]
    }")

  TX_COUNT=$(echo "$BLOCK" | jq '.result.transactions | length' 2>/dev/null)

  if [ "$TX_COUNT" = "null" ] || [ -z "$TX_COUNT" ] || [ "$TX_COUNT" = "0" ]; then
    echo "slot $SLOT skipped (no transactions or error)"
    SLOT=$((SLOT - 1))
    continue
  fi

  while IFS= read -r TX; do
    if [ $COUNT -ge $TARGET ]; then
      break
    fi
    if [ "$FIRST" = "1" ]; then
      echo "  \"$TX\"" >> "$OUTPUT"
      FIRST=0
    else
      echo "  ,\"$TX\"" >> "$OUTPUT"
    fi
    COUNT=$((COUNT + 1))
  done < <(echo "$BLOCK" | jq -r '.result.transactions[].transaction[0]')

  SLOT=$((SLOT - 1))
done

echo "]" >> "$OUTPUT"
echo "done. saved $COUNT transactions to $OUTPUT"
