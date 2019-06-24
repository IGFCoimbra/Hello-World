<?php

require_once ERROR_REPORTER_PATH;
assert_file_existence_and_require_once(APPPATH . 'controllers/bl_api/resource_controller.php');

class documents extends resource_controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('logic/bl_api/document_bl_api_logic');
        $this->model = $this->document_bl_api_logic;
    }

    public function get_index(string $matter_id) {
        if (!$this->helper_model->is_development_or_testing()) {
            $this->response_not_implemented();
        }
//        $entities = $this->model->get_index($matter_id);
        $entities = $this->model->get_index('delete me');
        $this->check_valid_output_format($entities, self::REQUEST_TYPE_GET_INDEX);
        $this->response($entities);
    }
    
}
