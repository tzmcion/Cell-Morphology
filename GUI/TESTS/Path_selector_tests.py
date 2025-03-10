from Display.path_selector import get_all_images
import os

def test_path_for_images():
    TEST_PATH = "GUI/TESTS/FILE_SAMPLES/TEST_PATHS"
    images = get_all_images(TEST_PATH)
    print(images)
    assert str(images[1]).endswith("/TT/bb.jpg")
    assert str(images[2]).endswith("/TT/DD/CC.jpg")
    assert str(images[0]).startswith(os.getcwd())