<?php
$upload_dir = 'C:/Users/PC/Desktop/testsite/achieve/';
if ($_FILES['file']['error'] === UPLOAD_ERR_OK) {
    $tmp_name = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    move_uploaded_file($tmp_name, $upload_dir . $name);
    echo "File uploaded!";
} else {
    echo "Upload failed!";
}
?>