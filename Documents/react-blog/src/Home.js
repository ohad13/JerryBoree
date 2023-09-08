import React, { useState } from 'react';
import BlogList from "./BlogList";
import useFetch from "./useFetch";
import LanguageFilter from './LanguageFilter';

const Home = () => {
  const { data: blogs, isPending, error } = useFetch('http://localhost:8000/blogs');
  const convertDateToTimestamp = (dateString) => {
    if (!dateString) { return 0; }
    const [day, month, year] = dateString.split('.').map(Number);
    return new Date(year, month - 1, day).getTime(); // Month is 0-based (0 = January)
  };
  const [sortBy, setSortBy] = useState('like'); // Default sorting by likes
  const [sortOrder, setSortOrder] = useState('desc'); // Default sorting order

  const sortBlogs = () => {
    if (!blogs) return;

    // Create a copy of the blogs array to avoid mutating the original
    const sortedBlogs = [...blogs];

    sortedBlogs.sort((a, b) => {
      if (sortBy === 'like') {
        return sortOrder === 'asc' ? a.like - b.like : b.like - a.like;
      }
      else if (sortBy === 'date') {
        const timestampA = convertDateToTimestamp(a.time);
        const timestampB = convertDateToTimestamp(b.time);
        return sortOrder === 'asc' ? timestampA - timestampB : timestampB - timestampA;
      }
      else if (sortBy === 'author') {
        return sortOrder === 'asc' ? a.author.localeCompare(b.author, undefined, { sensitivity: 'base' }) : b.author.localeCompare(a.author, undefined, { sensitivity: 'base' });
      }
      else if (sortBy === 'title') {
        return sortOrder === 'asc' ? a.title.localeCompare(b.title, undefined, { sensitivity: 'base' }) : b.title.localeCompare(a.title, undefined, { sensitivity: 'base' });
      }
      else if (sortBy === 'comment') {
        return sortOrder === 'asc' ? a.comments.length - b.comments.length : b.comments.length - a.comments.length;
      }
    });
    return sortedBlogs;
  };

  // Call the sorting function when the sortBy or sortOrder state changes
  const sortedBlogs = sortBlogs();

  // Function to toggle sorting order between ascending and descending
  const toggleSortOrder = () => {
    setSortOrder(sortOrder === 'asc' ? 'desc' : 'asc');
  };

  return (
    <div className="home">
      {error && <div>{error}</div>}
      {isPending && <div>Loading...</div>}
      <div style={{ display: 'flex' }}>
        {blogs && <LanguageFilter/>}
        {blogs && <div style={{marginLeft:'20px', marginBottom:"20px"}} className="sorting-controls">
          <label>Sort by: </label>
          <select value={sortBy} onChange={(e) => setSortBy(e.target.value)}>
            <option value="like">Likes</option>
            <option value="title">Title</option>
            <option value="author">Author</option>
            <option value="comment">Comments</option>
            <option value="date">Date</option>
          </select>
          <button onClick={toggleSortOrder}>
            {sortOrder === 'asc' ? 'Ascending' : 'Descending'}
          </button>
        </div>}</div>
      {sortedBlogs && <BlogList blogs={sortedBlogs} />}
    </div>
  );
}

export default Home;