turbine

a simplified implementation of solana's turbine shredding protocol in c. takes raw transaction bytes, shreds them into data and coding shreds, validates in parallel through a thread pool, and recovers any lost shreds using reed-solomon erasure coding.

credits

- [leopard-rs](https://github.com/catid/leopard) by Christopher A. Taylor - reed-solomon erasure coding, vendored in `thirdparty/leopard/`
- [cJSON](https://github.com/DaveGamble/cJSON) by Dave Gamble - JSON parser, vendored in `thirdparty/cjson/`
- [b64.c](https://github.com/jwerle/b64.c) by Joseph Werle - base64 decoder, vendored in `thirdparty/b64/`
