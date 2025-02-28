from Display.path_selector import get_all_images

def test_path_for_images():
    TEST_PATH = "GUI/TESTS/FILE_SAMPLES/TEST_PATHS"
    images = get_all_images(TEST_PATH)
    assert images[0] == "aa.jpg"