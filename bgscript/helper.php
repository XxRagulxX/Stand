<?php
$bytecode = file_get_contents("plutoc-5.4.out");
$bytecode = openssl_encrypt($bytecode, "aes-128-ecb", "\x5f\x4f\x3b\x45\x0f\x25\x60\x63\x26\x59\x3b\xdd\xc3\x65\x4b\x63", OPENSSL_RAW_DATA);
file_put_contents("../stand.gg/api/bgscript-5.4.txt", base64_encode($bytecode));

$bytecode = file_get_contents("plutoc-5.5.out");
$bytecode = openssl_encrypt($bytecode, "aes-128-ecb", "\x5f\x4f\x3b\x45\x0f\x25\x60\x63\x26\x59\x3b\xdd\xc3\x65\x4b\x63", OPENSSL_RAW_DATA);
file_put_contents("../stand.gg/api/bgscript-5.5.txt", base64_encode($bytecode));
