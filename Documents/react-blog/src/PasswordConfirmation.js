import React, { useState } from 'react';
import Modal from 'react-modal';

Modal.setAppElement('#root'); // Set the root element for accessibility

function PasswordConfirmation({ isOpen, onRequestClose, onConfirm }) {
  const [password, setPassword] = useState('');

  const handleConfirm = () => {
    if (password === '123') {
      onConfirm(); // Trigger the delete action
    } else {
      alert('Incorrect password. Please try again.');
    }
  };

  return (
    <Modal
      isOpen={isOpen}
      onRequestClose={onRequestClose}
      contentLabel="Password Confirmation"
    >
      <h2>Enter Password to Delete</h2>
      <input
        type="password"
        placeholder="Admin Password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />
      <button onClick={handleConfirm}>Confirm</button>
      <button onClick={onRequestClose}>Cancel</button>
    </Modal>
  );
}

export default PasswordConfirmation;
