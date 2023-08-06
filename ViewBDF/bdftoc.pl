#!/usr/bin/perl

# usage: ./bdftoc.pl < k8x12.bdf > font.h

$maxcode = 36000; # cut off higher codes due to maximum array size.

printf("// maxcode=%d\n", $maxcode);
print "const byte font[][12]={\n";
print " {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}//0(NULL)\n";

$table =  "const word fonttbl[65536] = {\n";
$table .= " 0 //0\n";

$i = 1;
$j = 1;
$infont = 0;
$num_char = 0;

while(<>){
    if(m/^ENCODING (\d+)/){
	$code = $1;
	last if($code > $maxcode); # cut off higher codes due to maximum array size.
	for( ; $j < $code; $j++){
	    $table .= sprintf(",0 //(%d,%04x)\n", $j, $j);
	}
	$table .= sprintf(",%d //(%d,%04x)\n", $i, $j, $j);
	$j++;
    }
    if(m/^BBX (.+) (.+) (.+) (.+)/){
	$bbx = $2;
	$shift = $3;
    }

    if(m/^BITMAP/){
	$font=",{";
	$x = 0;
	$infont = 1;
	for($x = 0; $x < 11-$bbx; $x++){
	    if($x != 0){
		$font .= ",";
	    }
	    $font .="0x00";
	}
    }
    if(infont){
	if(m/^([0-9a-fA-F]{2}$)/){
	    if($x != 0){
		$font .= ","
	    }
	    $val = hex($1) >> $shift;
	    $font.= sprintf("0x%02x", $val);
	    $x++;
	}
    }
    if(m/^ENDCHAR/){
	for($k = $x; $k < 12; $k++){
	    if($k != 0){
		$font .= ","
	    }
	    $font.= "0x00";
	}
	$font .= sprintf("}//%d(%d,%04x)\n", $i, $code, $code);
	print $font;
	$i++;
	$infont = 0;
    }
}
print "};\n";

for( ; $j < 65536; $j++){
    $table .= sprintf(",0 //(%d,%04x)\n", $j, $j);
}
$table .= "};\n";
print $table;

