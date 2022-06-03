"""Test suite for script utilities"""
from customer_parsers.sundance.scripts.common import filter_by_extension, partition_files


def test_filter_by_extension() -> None:
    assert filter_by_extension(['a.txt'], 'txt') == ['a.txt']
    assert filter_by_extension(['a.txt', 'b.pdf'], 'txt') == ['a.txt']
    assert not filter_by_extension(['a.txt'], 'pdf')


def test_partition_files() -> None:
    result1 = partition_files(['a.txt'], ['txt'])
    assert set(result1.keys()) == {'txt', 'other'}
    assert result1['txt'] == ['a.txt']
    assert not result1['other']

    result2 = partition_files(['a.txt', 'b.pdf'], ['txt'])
    assert set(result2.keys()) == {'txt', 'other'}
    assert result2['txt'] == ['a.txt']
    assert result2['other'] == ['b.pdf']

    result3 = partition_files(['a.txt', 'b.pdf'], ['txt', 'pdf'])
    assert set(result3.keys()) == {'txt', 'pdf', 'other'}
    assert result3['txt'] == ['a.txt']
    assert result3['pdf'] == ['b.pdf']
    assert not result3['other']
    
    
def test_filter_by_extension() -> None:
    assert filter_by_extension(['a.txt'], 'txt') == ['a.txt']
    assert filter_by_extension(['a.txt', 'b.pdf'], 'txt') == ['a.txt']
    assert not filter_by_extension(['a.txt'], 'pdf')
