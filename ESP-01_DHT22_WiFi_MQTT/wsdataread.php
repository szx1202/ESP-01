<?php
$fp = @fopen("wsData.txt", "r");
if ($fp) {
    while (($buffer = fgets($fp, 4096)) !== false) {   
        echo "<br>";        
        echo $buffer;
    }
    if (!feof($fp)) {
        echo "Error: unexpected fgets() fail\n";
    }
    fclose($fp);
}
?>