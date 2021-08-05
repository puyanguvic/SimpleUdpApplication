# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('simple-udp-application', ['core'])
    module.source = [
        'model/simple-udp-application.cc',
        'model/timestamp-tag.cc',
        'helper/simple-udp-application-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('simple-udp-application')
    module_test.source = [
        'test/simple-udp-application-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/simple-udp-application-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'simple-udp-application'
    headers.source = [
        'model/simple-udp-application.h',
        'model/timestamp-tag.h',
        'helper/simple-udp-application-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

