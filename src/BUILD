load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":serverlib",
        "//:hello_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_library(
    name = "serverlib",
    hdrs = ["//src:hello_server.hpp"],
    strip_include_prefix = "/src",
)

cc_binary(
    name = "client",
    srcs = ["client.cpp"],
    deps = [
        ":clientlib",
        "//:hello_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_library(
    name = "clientlib",
    hdrs = ["//src:hello_client.hpp"],
    strip_include_prefix = "/src",
)
