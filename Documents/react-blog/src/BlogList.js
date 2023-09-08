import { Link } from 'react-router-dom'
import React from 'react';
import { useLocation } from 'react-router-dom';

const BlogList = ({ blogs }) => {
  const location = useLocation();
  const queryParams = new URLSearchParams(location.search);
  const selectedLanguage = queryParams.get('language');
  const filteredBlogs = selectedLanguage ? blogs.filter(blog => blog.language === selectedLanguage) : blogs;

  return (
    <div className="blog-list">
      {filteredBlogs.map(blog => (
        <div className="blog-preview" key={blog.id} >
          <Link to={`/blogs/${blog.id}`}>
            <div style={{ display: 'flex', alignItems: 'center', marginBottom: '20px' }}>
              <div style={{ marginRight: '20px' }}>
                <h2>{blog.title}</h2>
                <p>Written by: {blog.author}</p>
                <p>Language:  {blog.language}</p>
                <p>Likes: {blog.like}</p>
                <p>Comments: {blog.comments.length}</p>
                <p>At: {blog.time}</p>
              </div>
              {blog.image && <img className="thumbnail-image" src={process.env.PUBLIC_URL + '/' + blog.image} alt="Blog Image" style={{ width: '200px', height: 'auto' }} />}
            </div>
          </Link>
        </div>
      ))}
    </div>
  );
}

export default BlogList;
