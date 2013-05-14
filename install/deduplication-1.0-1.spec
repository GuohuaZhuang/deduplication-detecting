# 文件名称: deduplication-1.0-1.spec
# 文件功能: deduplication软件包描述信息
# 文件作者: Firstboy
# 修改时间: 2012-07-20

Name: deduplication
Version: 1.0
Release: 1
Summary: 语义去重项目
Group: Applications/Engineering
License: Commercial
Vendor: 上海埃帕信息科技有限公司
Packager: 庄国华 (guohua.zhuang@ape-tech.com)
Source: deduplication-1.0.tar.gz
Prefix: /usr
Requires: /bin/sh
Provides: deduplication
# Buildroot: /tmp
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}

%description
deduplication语义去重采用Charikar的simhash思路，认为通过simhash编码后，以判断不同hashcode
之间不同bit位数的多少来表示语义相似程度。如果小于一定值，比如k为3位，这种情况下就可以近似认为两个
hashcode对应的文本内容段是语义相似的，达到语义去重中发现相似文档的功能。目前支持3000万文档到6000
万文档的数据量以内效果比较好。具体部署采用程序版主分布式搭建完成。

%prep
%setup
%configure
%build
make

%install
make DESTDIR=%{buildroot} install

%clean

%pre
echo ""

%post
echo ""

%preun
echo ""

%postun
echo ""

LIBDIR=$PREFIX/lib
%files
%defattr (-,root,root)
%{_libdir}
%{_libdir}/libdup.so
%{_libdir}/libdup.so.0
%{_libdir}/libdup.so.0.0.0
%{_libdir}/libdup.la
%{_libdir}/libdup.a
%{_bindir}
%{_bindir}/dup
%{_bindir}/dupserver
%{_bindir}/dupclient
%config /etc/init.d/dupd
%{_includedir}/dup
%{_includedir}/dup/common.h
%{_includedir}/dup/data.h
%{_includedir}/dup/dup.h
%{_includedir}/dup/lookup3.h
%{_includedir}/dup/simi.h
%{_includedir}/dup/simiw.h
%{_includedir}/dup/split.h
%{_includedir}/dup/util.h
%{_includedir}/dup/runtime.h
%{_includedir}/dup/redirect.h
%{_includedir}/dup/define.h
%{_includedir}/dup/conf.h
%{_includedir}/dup/server.h
%{_includedir}/dup/client.h
%{_includedir}/dup/scheduler.h
%{_includedir}/dup/serverm.h
%{_includedir}/dup/clientm.h
%{_includedir}/dup/hashmap.h
%config /etc/scholar2
%config /etc/scholar2/dup.client.conf
%config /etc/scholar2/dup.server.conf
%dir /ape/scholar2/deduplication
%dir /ape/scholar2/deduplication/index_table.dup

# /
# /deduplication-1.0/src/libdup.so
# %defattr (-,root,root)
# %{_libdir}/libdup.so
# %{_includedir}/dup/batch.h
# %{_includedir}/dup/client.h
# %{_includedir}/dup/clientm.h
# %{_includedir}/dup/common.h
# %{_includedir}/dup/conf.h
# %{_includedir}/dup/data.h
# %{_includedir}/dup/define.h
# %{_includedir}/dup/dup.h
# %{_includedir}/dup/hashmap.h
# %{_includedir}/dup/lookup3.h
# %{_includedir}/dup/redirect.h
# %{_includedir}/dup/runtime.h
# %{_includedir}/dup/scheduler.h
# %{_includedir}/dup/server.h
# %{_includedir}/dup/serverm.h
# %{_includedir}/dup/simi.h
# %{_includedir}/dup/simiw.h
# %{_includedir}/dup/split.h
# %{_includedir}/dup/util.h
# /ape/scholar2/deduplication/server.log
# /usr/sbin/dupserver
# /etc/init.d/dupd
# %config /etc/scholar2/dup.client.conf
# %config /etc/scholar2/dup.server.conf
# /ape/scholar2"/deduplication/index_table.dup


%changelog
* Fri Jul 20 2012 庄国华 <guohua.zhuang@ape-tech.com>
- 语义去重项目

