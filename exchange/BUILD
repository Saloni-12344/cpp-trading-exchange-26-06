load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "order",
    hdrs = ["order.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "order_book",
    hdrs = ["order_book.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":order",
        "//common:mem_pool",
    ],
)

cc_binary(
    name = "order_book_test",
    srcs = ["order_book_test.cpp"],
    deps = [":order_book"],
)
