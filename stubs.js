// Provides: caml_thread_self
function caml_thread_self() { }

// Provides: caml_thread_new
function caml_thread_new() { }

// Provides: caml_mutex_lock
function caml_mutex_lock() { }

// Provides: caml_mutex_unlock
function caml_mutex_unlock() { }

// Provides: ocaml_pa_initialize
function ocaml_pa_initialize() { }

// Provides: ocaml_pa_get_default_output_device
function ocaml_pa_get_default_output_device() { }

// Provides: ocaml_pa_get_device_info
function ocaml_pa_get_device_info() { 
    return {
        d_default_low_output_latency: 0,
    };
}

// Provides: ocaml_pa_open_stream_byte
function ocaml_pa_open_stream_byte() {
    return null;
};

// Provides: ocaml_pa_start_stream
function ocaml_pa_start_stream() { };

// Provides: ocaml_pa_stop_stream
function ocaml_pa_stop_stream() { };
