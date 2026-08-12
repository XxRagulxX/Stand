<?php
require __DIR__."/dbinclude.php";
require __DIR__."/geninclude.php";

function parseKey($key)
{
	if(substr($key, 0, 6) == "Stand-")
	{
		$key = substr($key, 6);
		if(substr($key, 0, 8) == "Upgrade-")
		{
			$key = substr($key, 8);
			if(strlen($key) == 31)
			{
				return ["upgrade", $key];
			}
		}
		else if(substr($key, 0, 6) == "Topup-")
		{
			$key = substr($key, 6);
			if(strlen($key) == 31)
			{
				return ["topup", $key];
			}
		}
		else
		{
			if(strlen($key) == 30)
			{
				return ["license", $key];
			}
		}
	}
	return ["", ""];
}

function isKeyTypeViableForTopup($type)
{
	return $type == "topup";
}

function getKeyCoinsValue($parsed_key)
{
	global $db;

	if($parsed_key[0] == "topup")
	{
		$res = $db->query("SELECT `coins` FROM `topup_keys` WHERE `key`=? AND `used_by`=''", "s", $parsed_key[1]);
		if(count($res) == 1)
		{
			return intval($res[0]["coins"]);
		}
	}
	return 0;
}

function setKeyUsedForTopup($parsed_key, $account_id)
{
	global $db;

	if($parsed_key[0] == "topup")
	{
		$db->query("UPDATE `topup_keys` SET `used_by`=? WHERE `key`=?", "ss", $account_id, $parsed_key[1]);
	}
}

function days_ago($d)
{
	return (time() - (60 * 60 * 24 * $d));
}

function pubSubSend($topic, $msg)
{
	$ch = curl_init();
	curl_setopt_array($ch, [
		CURLOPT_URL => "http://198.251.89.45/pub?topic=".$topic,
		CURLOPT_POST => true,
		CURLOPT_POSTFIELDS => $msg,
		CURLOPT_RETURNTRANSFER => true,
	]);
	curl_exec($ch);
	curl_close($ch);
}

function giv4_decode($gi)
{
	// GIV3Encoding
	$decodetbl = [ 'K' => 0, '2' => 1, 'N' => 2, 'P' => 3, 'F' => 4, 'Q' => 5, 'H' => 6, 'A' => 7, 'Z' => 8, 'L' => 9, 'X' => 10, 'T' => 11, 'J' => 12, 'G' => 13, 'B' => 14, 'V' => 15, '1' => 16, 'O' => 17, '5' => 18, 'Y' => 19, 'R' => 20, 'S' => 21, 'W' => 22, 'M' => 23, '8' => 24, '0' => 25, '3' => 26, '7' => 27, '6' => 28, '4' => 29, 'E' => 30, 'D' => 31, '9' => 32, 'U' => 33, 'I' => 34, 'C' => 35 ];
	$int = gmp_init(0);
	foreach (str_split($gi) as $c)
	{
		$int = gmp_mul($int, 36);
		$int = gmp_add($int, $decodetbl[$c]);
	}
	$str = gmp_export($int);
	while (strlen($str) < 8)
	{
		$str = "\0".$str;
	}
	$xored_and_scrambled_rid = unpack("J", $str)[1];
	$word_0 = ($xored_and_scrambled_rid >> 48);
	$word_1 = ($xored_and_scrambled_rid >> 32) & 0xffff;
	$word_2 = ($xored_and_scrambled_rid >> 16) & 0xffff;
	$word_3 = $xored_and_scrambled_rid & 0xffff;
	// rid_reversible_scramble
	$word_0 ^= $word_3;
	$word_1 ^= $word_3;
	$word_2 ^= $word_3;
	// MAGIC_GI_V4_RID
	$word_0 ^= 0x872e;
	$word_1 ^= 0x797c;
	$word_2 ^= 0xd366;
	$word_3 ^= 0x135d;
	return ($word_0 << 48) | ($word_1 << 32) | ($word_2 << 16) | $word_3;
}
